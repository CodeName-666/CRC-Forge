# CRC

**CRC Forge** ist der Paketname dieser Library in den PlatformIO-Metadaten.

Portable C++11-Library für CRC-8/SAE-J1850, CRC-8/AUTOSAR (H2F),
CRC-16/IBM-3740 (CCITT-FALSE) und CRC-32/ISO-HDLC.
Die Library benötigt weder Arduino noch dynamische Speicherallokation.
Sie unterstützt synchrone, blockweise und schrittweise Berechnung.

## Installation mit PlatformIO

Das [GitHub-Repository](https://github.com/CodeName-666/CRC-Forge) kann direkt
als PlatformIO-Abhängigkeit eingebunden werden:

```ini
[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = https://github.com/CodeName-666/CRC-Forge.git
build_flags = -DCRC_TABLE_SIZE=256
```

Für lokale Entwicklung den Library-Ordner nach `lib/CRC` kopieren oder
`lib_deps = symlink://D:/Projekte/CRC/crc` mit dem eigenen Pfad verwenden.
`Crc.h`, `Crc.cpp`,
`Crc_Cfg.h` und `Crc_Types.h` liegen im Wurzelverzeichnis der Library.
Eine Veröffentlichung in der PlatformIO Registry wird nicht vorausgesetzt.

## Algorithmus auswählen

`Crc` ist die zentrale Bridge. `Algorithm_E` und die zugehörigen Enum-Werte
stehen ohne Namespace direkt in `Crc_Types.h`.

```cpp
#include <Crc.h>

const uint8_t data[] = "123456789";
const Algorithm_E algorithm = CRC_32;
const uint32_t value = Crc::calculate(algorithm, data, 9U);
// value == 0xCBF43926
```

| Enum-Wert | Verfahren | Polynom | Init | RefIn/RefOut | XorOut | Prüfsumme für „123456789“ |
| --- | --- | --- | --- | --- | --- | --- |
| `CRC_8` | SAE-J1850 | 0x1D | 0xFF | nein/nein | 0xFF | 0x4B |
| `CRC_8H2F` | AUTOSAR | 0x2F | 0xFF | nein/nein | 0xFF | 0xDF |
| `CRC_16` | IBM-3740 / CCITT-FALSE | 0x1021 | 0xFFFF | nein/nein | 0 | 0x29B1 |
| `CRC_32` | ISO-HDLC | 0x04C11DB7 | 0xFFFFFFFF | ja/ja | 0xFFFFFFFF | 0xCBF43926 |

CRC32 verwendet intern das reflektierte Polynom `0xEDB88320`.
Die Bridge bietet außerdem `calculateCrc8()`, `calculateCrc8H2F()`,
`calculateCrc16()` und `calculateCrc32()` für die gezielte statische Berechnung.

## Schrittweise in der Superloop

Dieses vollständige Arduino-Beispiel verarbeitet pro `loop()`-Aufruf höchstens
ein Byte. Zwischen den Aufrufen kann die Anwendung andere Aufgaben erledigen.

```cpp
#include <Arduino.h>
#include <Crc.h>

static uint8_t sData[] = "123456789";
static Crc sChecksum;

void setup() {
    Serial.begin(115200UL);
    sChecksum.setType(CRC_32);
    sChecksum.init(sData, 9U);
    if (!sChecksum.start()) {
        Serial.println(F("CRC start failed"));
    }
}

void loop() {
    sChecksum.process();
    const CalculationStatus_E status = sChecksum.getStatus();
    if (status == CRC_CALC_FINISHED) {
        Serial.println(sChecksum.get(), HEX); // CBF43926, printed once
    }
}
```

Der Puffer gehört dem Aufrufer. Er muss während der Berechnung gültig und
unverändert bleiben; die Library kopiert oder verändert ihn nicht.

| Methode | Verhalten |
| --- | --- |
| `init(pData, dataLen)` | Setzt Puffer und Länge; verwirft Fortschritt und Ergebnis. Der gewählte Algorithmus bleibt erhalten. |
| `setType(algorithm)` | Wählt den Algorithmus der Bridge; verwirft Fortschritt und Ergebnis. |
| `start()` | Startet aus dem Ruhezustand; liefert bei ungültigem oder leerem Puffer sowie deaktiviertem Algorithmus `false`. |
| `process()` | Verarbeitet höchstens ein Byte. Im Ruhe- oder Abschlusszustand erfolgt keine Änderung. |
| `getStatus()` | Liefert direkt `CalculationStatus_E`: `CRC_NO_CALC`, `CRC_CALC_ACTIVE` oder `CRC_CALC_FINISHED`. |
| `isFinished()` | Prüft, ob ein fertiges Ergebnis vorliegt. |
| `get()` | Holt ein fertiges Ergebnis ab und setzt den Zustand auf `CRC_NO_CALC`; vor Abschluss ist der Rückgabewert 0. |
| `cancel()` | Bricht aktive Arbeit ab und liefert `true`; erhält Puffer und Länge. Ein fertiges Ergebnis bleibt erhalten. |
| `calculate()` | Berechnet den konfigurierten Puffer synchron; verändert den schrittweisen Berechnungszustand nicht. |

`setDataPtr()` und `setDataLen()` ändern einzelne Konfigurationswerte und
verwerfen ebenfalls Fortschritt und Ergebnis. `getDataPtr()` und `getDataLen()`
lesen die Konfiguration. `loop()` führt denselben Schritt wie `process()` aus.
Das letzte verarbeitete Byte setzt unmittelbar `CRC_CALC_FINISHED`.

## CRC-Klassen direkt verwenden

Für einen feststehenden Algorithmus kann die jeweilige Klasse ohne Bridge
verwendet werden: `Crc8`, `Crc8H2F`, `Crc16` oder `Crc32`.

```cpp
#include <src/Crc16.h>

uint8_t data[] = "123456789";
const uint32_t direct = Crc16::calculate(data, 9U); // 0x29B1

Crc16 checksum;
checksum.init(data, 9U);
const uint32_t configured = checksum.calculate(); // 0x29B1
```

Die direkten Klassen bieten dieselbe Pufferkonfiguration und Zustandsverwaltung
wie die Bridge, jedoch kein `setType()`. Auch der Konstruktor kann Puffer und
Länge übernehmen: `Crc16 checksum(data, 9U)`.

## Nachrichten blockweise verarbeiten

Für eine Fortsetzung wird die zuvor zurückgegebene, bereits finalisierte
Prüfsumme übergeben. Die Parameterreihenfolge unterscheidet sich zwischen
Bridge und direkter Klasse:

```cpp
#include <Crc.h>

const uint8_t data[] = "123456789";

// Bridge: firstCall, startValue
uint32_t bridged = Crc::calculate(CRC_16, data, 4U);
bridged = Crc::calculate(CRC_16, data + 4U, 5U, false, bridged);

// Direct class: startValue, isFirstCall
uint32_t direct = Crc16::calculate(data, 4U);
direct = Crc16::calculate(data + 4U, 5U, direct, false);
// Both results are 0x29B1.
```

Beim ersten Aufruf gilt der feste Init-Wert des Verfahrens; ein übergebener
Startwert wird ignoriert. Bei Fortsetzungen müssen Verfahren und Reihenfolge
der Datenblöcke gleich bleiben.

Synchrone Aufrufe erlauben leere Blöcke, auch mit `nullptr`: Der erste leere
Block ergibt 0 für CRC8/H2F/32 beziehungsweise 0xFFFF für CRC16. Ein leerer
Folgeblock erhält die vorherige Prüfsumme innerhalb der jeweiligen CRC-Breite.
Ein Nullpointer mit positiver Länge sowie unbekannte oder deaktivierte
Bridge-Algorithmen liefern 0. Da 0 eine gültige Prüfsumme sein kann, ist dieser
Wert kein eindeutiger Fehlerindikator.

## CPU oder Tabellen konfigurieren

Alle Einstellungen stehen in `Crc_Cfg.h` und können projektweit mit
`build_flags` überschrieben werden. Standardmäßig sind alle vier Verfahren
aktiv und verwenden Tabellen mit 256 Einträgen.

| Einstellung | Berechnung | Tabellenspeicher für alle vier Verfahren |
| --- | --- | --- |
| `CRC_TABLE_SIZE=0` | Acht bitweise Schritte je Byte | 0 Bytes |
| `CRC_TABLE_SIZE=16` | Zwei Tabellenzugriffe je Byte | 128 Bytes |
| `CRC_TABLE_SIZE=256` | Ein Tabellenzugriff je Byte | 2048 Bytes |

Diese Größen enthalten nur Tabellen, keinen Programmcode. Auf AVR liegen die
Tabellen im Flash (`PROGMEM`). Einzelne Backends können unabhängig gewählt werden:

```ini
build_flags =
    -DCRC_TABLE_SIZE=16
    -DCRC32_TABLE_SIZE=256
    -DCRC8H2F_TABLE_SIZE=0
    -DCFG_CRC8_ENABLE=0
```

| Verfahren | Aktivierung mit 0 oder 1 | Individuelle Tabellengröße |
| --- | --- | --- |
| CRC8 | `CFG_CRC8_ENABLE` | `CRC8_TABLE_SIZE` |
| CRC8H2F | `CFG_CRC8H2F_ENABLE` | `CRC8H2F_TABLE_SIZE` |
| CRC16 | `CFG_CRC16_ENABLE` | `CRC16_TABLE_SIZE` |
| CRC32 | `CFG_CRC32_ENABLE` | `CRC32_TABLE_SIZE` |

Für Tabellengrößen sind ausschließlich 0, 16 und 256 erlaubt. Deaktivierte
Verfahren werden einschließlich ihrer Tabellen nicht eingebunden. Direkte
statische Berechnungen dieser Klassen dürfen dann nicht aufgerufen werden;
die Bridge liefert 0 beziehungsweise bei `start()` den Wert `false`.

Die ebenfalls unterstützten `CRCx_ENABLED`-Defines dürfen den
`CFG_CRCx_ENABLE`-Werten nicht widersprechen. Ungültige Werte führen zu einem
Compilerfehler. Einstellungen müssen für alle Übersetzungseinheiten gelten;
ein lokales `#define` im Anwendungscode konfiguriert die separat kompilierte
Library nicht.

## Aufbau und Embedded-Eigenschaften

- `Crc.h` / `Crc.cpp`: Bridge mit Enum-Auswahl und direkter Weiterleitung an statische CRC-Funktionen.
- `src/Crc8.*`, `src/Crc8H2F.*`, `src/Crc16.*`, `src/Crc32.*`: Rechenkerne und Tabellen.
- `src/CrcIf.h`: gemeinsame Konfiguration und Zustandsverwaltung als Template `CrcIf<Algorithm>`.
- `Crc_Types.h`: `Algorithm_E`, `CalculationStatus_E`, `BufferConfiguration_T` und `CalculationState_T`, ohne Namespace oder Typaliase.
- `Crc_Cfg.h`: Build-Konfiguration, Konstanten und abstrahierte Tabellenzugriffe.

Jede Instanz besitzt einen Berechnungszustand. Die Template-Aufrufe werden
statisch gebunden; Heap-Allokationen, virtuelle Methoden und manuelle
Objektlebensdauerverwaltung sind nicht erforderlich. Kopien übernehmen den
Zustand, teilen jedoch den vom Aufrufer verwalteten Eingabepuffer.

Zielplattformen sind 8-Bit-AVR und 32-Bit-ESP32 mit eingeschränktem C++11.
Die Library rechnet ausschließlich mit Ganzzahlen; eine FPU wird nicht benötigt.
32-Bit-Wertparameter werden auch auf AVR verwendet. Enums haben `uint8_t`
als Basistyp. Exceptions und RTTI sind für den Library-Build deaktiviert;
strenge Compilerwarnungen prüfen unter anderem Typumwandlungen.

Die API ist für Superloop oder Task-Kontext vorgesehen, nicht für ISRs.
Gemeinsame Instanzen benötigen externe Synchronisierung. Synchrone Aufrufe
verarbeiten den gesamten Block; ihre Laufzeit hängt von der Datenlänge ab.
Anwendungen verwenden die direkten Typnamen und Enum-Werte und müssen nach
API-Änderungen vollständig neu gebaut werden. Die Regeln orientieren sich am
Embedded-C/C++-Skill; eine MISRA-Zertifizierung wird nicht behauptet.

## Beispiele und Prüfung

| Beispiel | Inhalt | Umgebungen |
| --- | --- | --- |
| `examples/Basic` | CRC32 über `Algorithm_E`, synchrone und blockweise Referenzprüfung, danach `init()` / `start()` / `process()` | `native`, `uno_cpu`, `uno_small`, `uno_large`, `esp32` |
| `examples/Direct` | CRC16 ohne Bridge; andere Verfahren deaktiviert, Tabelle mit 16 Einträgen | `native`, `uno`, `esp32` |

Arduino-Beispiele geben das Ergebnis einmalig mit 115200 Baud aus. Fehler
beim Start oder bei der Referenzprüfung werden gemeldet. Die Hostprogramme
prüfen alle drei Berechnungsarten und liefern bei Fehlern Exitcode 1.

Vom Wurzelverzeichnis der Library aus:

```sh
pio test
pio run -d examples/Basic
pio run -d examples/Basic -e native -t exec
pio run -d examples/Direct
pio run -d examples/Direct -e native -t exec
pio pkg pack -o CRCForge-1.0.0.tar.gz
```

Native Builds benötigen GCC/G++ im PATH. Die Tests decken Referenzwerte,
Blockgrenzen, ungültige Eingaben, Zustandswechsel, Kopien sowie CPU-, Tabellen-
und deaktivierte Konfigurationen ab. Builds ersetzen keinen Test auf echter
Hardware.

## Lizenz

Autor: Christof Seidel.

Diese Library steht unter der [MIT-Lizenz](LICENSE).
Copyright (c) 2026 Christof Seidel.

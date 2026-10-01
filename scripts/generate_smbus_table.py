"""Generate CRC-8/SMBUS nibble/byte tables; run from any working directory."""
from pathlib import Path


def remainder(value):
    """Divide a byte followed by eight zero bits by the SMBUS polynomial."""
    for _ in range(8):
        value = ((value << 1) ^ (0x07 if value & 0x80 else 0)) & 0xFF
    return value


def render():
    """Render the array; its first 16 entries also form the nibble table."""
    lines = ["static const uint8_t Crc8Smbus_Table[CRC8_SMBUS_TABLE_SIZE] CRC_TABLE_STORAGE = {"]
    for offset in range(0, 256, 8):
        if offset == 16:
            lines.append("#if CRC8_SMBUS_TABLE_SIZE == 256")
        lines.append("    " + ", ".join(f"0x{remainder(i):02X}U" for i in range(offset, offset + 8)) + ",")
    lines.extend(["#endif", "};"])
    return "\n".join(lines)


if __name__ == "__main__":
    target = Path(__file__).resolve().parents[1] / "src/Crc8Smbus.cpp"
    source = target.read_text(encoding="utf-8")
    begin = "// BEGIN GENERATED SMBUS TABLE\n"
    end = "// END GENERATED SMBUS TABLE"
    prefix, rest = source.split(begin, 1)
    _, suffix = rest.split(end, 1)
    target.write_text(prefix + begin + render() + "\n" + end + suffix, encoding="utf-8")

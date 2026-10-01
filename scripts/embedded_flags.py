"""Apply restricted C++ checks without changing PlatformIO-generated C glue."""
Import("env")
env.Append(CXXFLAGS=["-fno-exceptions", "-fno-rtti", "-Wconversion",
                     "-Wsign-conversion", "-Wundef", "-Wdouble-promotion"])

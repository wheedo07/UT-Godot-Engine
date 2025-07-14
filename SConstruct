#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["cpp/"])
sources = Glob("cpp/engine/undertale_engine.cpp")
sources += Glob("cpp/src/register_types.cpp")
sources += Glob("cpp/env.cpp")
sources += Glob("cpp/engine/*/*/*/*.cpp")
sources += Glob("cpp/engine/*/*/*.cpp")
sources += Glob("cpp/engine/*/*.cpp")
sources += Glob("cpp/src/*/*.cpp")
sources += Glob("cpp/src/*/*/*.cpp")
sources += Glob("cpp/src/*/*/*/*.cpp")

library = env.SharedLibrary(
    "godot/bin/exe/lib.UndertaleEngine{}{}".format(env["suffix"], env["SHLIBSUFFIX"]).replace(".template", ""),
    source=sources,
)

Default(library)
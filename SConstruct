#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")
suffix = [ ".template", ".dev" ]
# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["cpp/"])

sources = Glob("cpp/engine_main.cpp")
sources += Glob("cpp/undertale_register.cpp")
sources += Glob("cpp/undertale-engine/*/*/*/*.cpp")
sources += Glob("cpp/undertale-engine/*/*/*.cpp")
sources += Glob("cpp/undertale-engine/*/*.cpp")

# if env["target"] in ["editor", "template_debug"]:
#     sources_doc = Glob("doc_classes/*/*.xml")
#     doc_data = env.GodotCPPDocData("cpp/engine_doc.gen.cpp", sources_doc)
#     sources.append(doc_data)

path = "godot/bin/lib/lib.UndertaleEngine{}{}".format(env["suffix"], env["SHLIBSUFFIX"])
for s in suffix:
    path = path.replace(s, "")

library = env.SharedLibrary(
    path,
    source=sources,
)

Default(library)
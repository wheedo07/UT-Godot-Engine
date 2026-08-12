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

libName = {
    "undertale-engine": "UndertaleEngine",
    "undertale-plugin": "UndertalePlugin"
}

if(env["target"] == "editor"):
    lib = "undertale-plugin"
    sources = Glob("cpp/{}/plugin_main.cpp".format(lib))
else:
    lib = "undertale-engine"
    sources = Glob("cpp/{}/engine_main.cpp".format(lib))
sources += Glob("cpp/{}/register.cpp".format(lib))
sources += Glob("cpp/{}/*/*/*/*.cpp".format(lib))
sources += Glob("cpp/{}/*/*/*.cpp".format(lib))
sources += Glob("cpp/{}/*/*.cpp".format(lib))
sources += Glob("cpp/env.cpp")

# if lib == "undertale-engine" and env["target"] in ["editor", "template_debug"]:
#     sources_doc = Glob("doc_classes/*/*.xml")
#     doc_data = env.GodotCPPDocData("cpp/{}/engine_doc.gen.cpp".format(l), sources_doc)
#     sources.append(doc_data)

path = "godot/bin/lib/lib.{}{}{}".format(libName[lib], env["suffix"], env["SHLIBSUFFIX"])
for s in suffix: path = path.replace(s, "")
library = env.SharedLibrary(
    path,
    source=sources,
)
Default(library)
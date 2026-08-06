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

lib = ["undertale-engine", "undertale-plugin"]
libName = {
    "undertale-engine": "UndertaleEngine",
    "undertale-plugin": "UndertalePlugin"
}

for l in lib:
    if(l == "undertale-engine"):
        sources = Glob("cpp/{}/engine_main.cpp".format(l))
    else:
        sources = Glob("cpp/{}/plugin_main.cpp".format(l))
    sources += Glob("cpp/{}/register.cpp".format(l))
    sources += Glob("cpp/{}/*/*/*/*.cpp".format(l))
    sources += Glob("cpp/{}/*/*/*.cpp".format(l))
    sources += Glob("cpp/{}/*/*.cpp".format(l))
    sources += Glob("cpp/env.cpp")

    # if l == "undertale-engine" and env["target"] in ["editor", "template_debug"]:
    #     sources_doc = Glob("doc_classes/*/*.xml")
    #     doc_data = env.GodotCPPDocData("cpp/{}/engine_doc.gen.cpp".format(l), sources_doc)
    #     sources.append(doc_data)

    path = "godot/bin/lib/lib.{}{}{}".format(libName[l], env["suffix"], env["SHLIBSUFFIX"])
    for s in suffix:
        path = path.replace(s, "")

    library = env.SharedLibrary(
        path,
        source=sources,
    )
    Default(library)
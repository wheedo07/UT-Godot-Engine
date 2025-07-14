#!/bin/bash
is=0
debug_mode=1

# source ~/start/.emsdk/emsdk_env.sh

target="template_debug"
exports="debug"
if (( $is == 1 )); then
    target="template_release"
    exports="release"
fi

if ls godot/bin/exe/~* 1> /dev/null 2>&1; then
    find "godot/bin/exe/"* -type f -exec rm {} \;;
fi
scons platform=windows build_library=no target=$target

mode="Windows"
if (( $debug_mode == 1)); then
    mode="Windows_DebugMode"
fi

# rm -r result/*; scons build_library=no && godot --headless --path ./godot/ -v --export-$exports $mode
#!/bin/sh

# 1. Get the directory where THIS script actually lives (Universal POSIX way)
SCRIPT_DIR=$(dirname "$0")

# 2. Change to that directory so relative asset paths work perfectly
cd "$SCRIPT_DIR" || exit 1

# 3. Tell the system to look inside our bundled 'lib' folder for SDL3
export LD_LIBRARY_PATH="./lib:$LD_LIBRARY_PATH"

# 4. Launch the hidden binary and pass along any arguments
exec ./griddy.bin "$@"

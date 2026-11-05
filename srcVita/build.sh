#!/bin/sh
set -eu

here=$(cd "$(dirname "$0")" && pwd)

if [ -z "${VITASDK:-}" ]; then
    echo "VITASDK is not set. Point it at the VitaSDK install and run this again." >&2
    exit 1
fi

if ! command -v arm-vita-eabi-gcc >/dev/null 2>&1; then
    echo "arm-vita-eabi-gcc is not on PATH. Add \$VITASDK/bin to PATH." >&2
    exit 1
fi

make -C "$here" all
echo "$here/build/ghosttrick.vpk"

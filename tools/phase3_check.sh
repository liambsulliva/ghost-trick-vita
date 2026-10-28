#!/bin/sh
set -e

if [ "$#" -ne 1 ]; then
    echo "usage: tools/phase3_check.sh <your rom>.nds"
    exit 1
fi

REPO_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BASE_ROM="$1"
DSD="$REPO_DIR/toolchain/dsd"
CONFIG_DIR="$REPO_DIR/config/arm9"
EXTRACT_DIR="$REPO_DIR/extracted/dsd/extract"
OBJECTS_DIR="$REPO_DIR/extracted/dsd/build/delinks"
LLD_DIR="$REPO_DIR/extracted/lld"
REBUILD_DIR="$REPO_DIR/extracted/rebuild"
REBUILT_ROM="$REPO_DIR/extracted/rebuilt.nds"

cd "$REPO_DIR"

"$DSD" delink --config-path "$CONFIG_DIR/config.yaml" > "$REPO_DIR/extracted/dsd/delink.log" 2>&1
python3 -I tools/lld_link.py "$CONFIG_DIR" "$OBJECTS_DIR" "$EXTRACT_DIR" "$LLD_DIR" > "$LLD_DIR.log" 2>&1 || {
    grep -E '^(PASS|FAIL|link failed)' "$LLD_DIR.log"
    echo "module check failed, see $LLD_DIR.log"
    exit 1
}
grep -E '^(PASS|FAIL|all )' "$LLD_DIR.log"

rm -rf "$REBUILD_DIR"
cp -R "$EXTRACT_DIR" "$REBUILD_DIR"
cp "$LLD_DIR/main.bin" "$REBUILD_DIR/arm9/arm9.bin"
cp "$LLD_DIR/itcm.bin" "$REBUILD_DIR/arm9/itcm.bin"
cp "$LLD_DIR/dtcm.bin" "$REBUILD_DIR/arm9/dtcm.bin"
for overlay in "$LLD_DIR"/ov*.bin; do
    cp "$overlay" "$REBUILD_DIR/arm9_overlays/$(basename "$overlay")"
done

"$DSD" rom build --config "$REBUILD_DIR/config.yaml" --rom "$REBUILT_ROM" > "$REPO_DIR/extracted/rebuild.log" 2>&1
python3 -I tools/fix_header.py "$BASE_ROM" "$REBUILT_ROM"

BASE_SHA="$(shasum "$BASE_ROM" | cut -d' ' -f1)"
REBUILT_SHA="$(shasum "$REBUILT_ROM" | cut -d' ' -f1)"
echo "base ROM    $BASE_SHA"
echo "rebuilt ROM $REBUILT_SHA"
if [ "$BASE_SHA" = "$REBUILT_SHA" ]; then
    echo "phase 3 baseline passed: the ROM rebuilds byte-identical from linked objects"
else
    echo "phase 3 baseline FAILED: hashes differ"
    exit 1
fi

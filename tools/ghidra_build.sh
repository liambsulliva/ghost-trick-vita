#!/bin/sh
set -e

REPO_DIR="$(cd "$(dirname "$0")/.." && pwd)"
NDS_DIR="$REPO_DIR/extracted/nds"
OUT_DIR="$REPO_DIR/extracted/ghidra"
DATABASE="$REPO_DIR/symbols/database.txt"
HEADLESS="$(brew --prefix ghidra)/libexec/support/analyzeHeadless"
JAVA_HOME="$(brew --prefix openjdk@25)/libexec/openjdk.jdk/Contents/Home"
export JAVA_HOME

mkdir -p "$OUT_DIR"
mkdir -p "$REPO_DIR/symbols"

if [ ! -f "$DATABASE" ]; then
    echo "no symbols/database.txt yet, running full analysis"
    "$HEADLESS" "$OUT_DIR" GhostTrickAnalysis \
        -import "$NDS_DIR/segments/main.bin" \
        -overwrite \
        -loader BinaryLoader \
        -loader-baseAddr 0x02000000 \
        -loader-blockName main \
        -processor ARM:LE:32:v5t \
        -scriptPath "$REPO_DIR/tools/ghidra" \
        -preScript LoadNdsMap.java "$NDS_DIR" \
        -postScript ExportText.java "$OUT_DIR/database_analysis.txt"
    cp "$OUT_DIR/database_analysis.txt" "$DATABASE"
fi

"$HEADLESS" "$OUT_DIR" GhostTrick \
    -import "$NDS_DIR/segments/main.bin" \
    -overwrite \
    -noanalysis \
    -loader BinaryLoader \
    -loader-baseAddr 0x02000000 \
    -loader-blockName main \
    -processor ARM:LE:32:v5t \
    -scriptPath "$REPO_DIR/tools/ghidra" \
    -preScript LoadNdsMap.java "$NDS_DIR" \
    -preScript ApplyExport.java "$DATABASE" \
    -postScript ExportText.java "$OUT_DIR/database_rebuilt.txt"

if cmp -s "$DATABASE" "$OUT_DIR/database_rebuilt.txt"; then
    echo "rebuild proven: symbols/database.txt rebuilds to an identical export"
else
    echo "rebuild NOT proven: exports differ"
    diff "$DATABASE" "$OUT_DIR/database_rebuilt.txt" | head -20
    exit 1
fi

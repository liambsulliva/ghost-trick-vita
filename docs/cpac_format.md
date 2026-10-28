# CPAC keys and resource lookup

Source: the user's USA DS dump (`BGTE`), the extracted packs, and ARM9 entry loader `func_0201e664`. Generated entries and manifests belong under ignored `extracted/`.

## Block layout

A block starts with two u32 fields, then `YEKP` or `YEKB`, a u32 key offset, `TADP` or `TADB`, and a u32 data offset. These are little-endian fields and offsets relative to the block base. The reversed tags correspond to `PKEY`/`BKEY` and `PDAT`/`BDAT`. The key table lies between the key and data offsets.

`P` keys occupy four bytes. For key value `v`:

- Data offset: `(v >> 16) * 32`, relative to the block's data start.
- Stored size: `((v >> 8) & 0xff) * 32`.
- Bit `0x80`: the start of a language-dependent group, not compression.

`B` keys occupy eight bytes, two u32 words:

- Data offset: `first & 0x7fffffff`, relative to the data start.
- Stored size: `second & 0x7fffffff`.
- Bit `0x80000000` in the first word: language-dependent group.
- Bit `0x80000000` in the second word: compressed data.

The game reads up to five adjacent keys for a language-dependent group and selects one using its language setting. The extractor keeps every nonempty physical entry under its original index. It records the group flag as `localized`; it does not select a language or collapse adjacent variants. This pack-level selection is separate from the resource-ID language table in ARM9.

Zero-size keys are omitted. Entry bounds must fit before the next block, or the end of the pack. Flagged compressed entries use the existing DS LZ decoder; `status` records whether decompression succeeded or the stored bytes were kept. The manifest appends `key_size` and `localized` to the previous columns. It uses CSV quoting with tab delimiters so a quote at the beginning of a four-byte magic string cannot swallow following rows.

## Extractor fix and validation

The old extractor treated every key as eight bytes. It therefore paired unrelated palette keys as offset and size, producing one bogus block 0 entry. It also treated the language bit in a byte offset as part of that offset, discarding flagged entries as out of bounds. A synthetic pack with one four-byte palette key reproduced the first failure before the fix.

Fresh extraction on 2026-10-28 produced:

- CPAC 2D: five blocks, 24,853 nonempty entries. Counts by block are 2,905, 8, 307, 21,477 and 156.
- CPAC 3D: four blocks, 1,481 nonempty entries. Counts by block are 1,136, 0, 10 and 335.
- All 20,356 previously extracted 2D byte-key files and 1,292 3D files remain byte-identical in the new output.
- All 247 loads in `extracted/trace/cpac_loads.tsv` have extracted files and manifest rows. The trace's pack pointers distinguish the two packs.

The original output remains in `extracted/cpac/` for comparison. The checked output is separate:

```
python3 -I tools/cpac_unpack.py extracted/nds/files/cpac_2d.bin extracted/cpac_checked
python3 -I tools/cpac_unpack.py extracted/nds/files/cpac_3d.bin extracted/cpac_checked
python3 -I -m unittest discover -s tests
```

Nine synthetic CPAC tests cover both key sizes, raw palettes, independent language and compression flags, block bounds, truncated markers and manifest quoting. They contain no game data.

`tools/cell_resources.py` resolves an ARM9 cell resource ID into palette, graphics, cell and animation indices. Its format and the neon class bindings are described in `docs/frame_mapping.md`. Resource IDs and CPAC entry indices are distinct namespaces.

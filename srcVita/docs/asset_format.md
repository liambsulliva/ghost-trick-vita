# Converted asset format

The converter turns the remaster's Android files into small, simple files the Vita app can load. Everything it produces stays out of git, under `extracted/vita_assets/<set>/` on the Mac and `ux0:data/ghosttrick/assets/` on the Vita.

```
manifest.txt
textures/<texture_name>.gtex
animations/<animation_name>.anim
scenes/<scene_name>.scene
music/<track_name>.ogg
music/<track_name>.loop
sounds/<sound_name>.ogg
```

Names use only letters, digits, `_` and `-`, and are at most 95 characters. All text files use one command per line, with fields separated by a single space.

## Vocabulary

| Word | Meaning |
|---|---|
| sheet | The object in a source timeline that lists its pages, atlas size, cell size and how many cells each page holds |
| page | One source texture cut into a grid of cells. An animation can use several |
| cell | One rectangle of that grid, holding one drawing |
| record | One entry of a source timeline: which cell to draw, and where |
| frame | One entry of an `.anim` file, written by the converter from one record |
| DS frame | One frame of the DS game, lasting 4 ticks. A timeline has 1, 2 or 4 records per DS frame |
| pose | An animation placed at an exact position, held for a check or a set description |
| `.gtex` | The texture file the converter writes |
| background | A texture the manifest lists for the viewer to browse |
| plate | A texture a scene places in a room, at a position in DS pixels |
| grain | One block of audio frames handed to an audio port: 4096 frames for music, 512 for sounds |
| cue | A `music`, `music_stop` or `sound` step, placed by splitting a `wait` |
| text line | One line of narration, rendered by the converter into its own `.gtex` |
| overlay | A bubble texture a scene places in a room, drawn above the objects |

## Coordinates

The DS game logic works in DS pixels, so positions in these files do too. The remaster's art is 4 times the size of DS pixels.

One DS pixel is 2.8125 screen pixels on the Vita. Art is converted to exactly that scale wherever possible, which means 0.703125 times its remaster size.

The DS camera covers 256 by 192 DS pixels, which is 720 by 540 screen pixels. The Vita's screen is 960 by 544, so it shows more of the room than the DS did, about 341 by 193 DS pixels. Scene files still give the DS camera, and the app works out the wider view from it:

- The art around the camera is every plate that overlaps the DS camera's 256 by 192 window. A plate that only touches that window does not count.
- If the art is bigger than the screen, the DS window stays centred on screen, and the view slides to stay inside the art. Nothing is stretched, and the screen is always full.
- If the art is smaller than the screen, it is centred with black around it. Room 11's close-up plate and the chapter card are like this, because no art exists beyond their edges.
- While the mask is on and no iris is open, only the beams can be seen, so the DS window stays centred on screen whatever the size of the art. A beam then keeps its place on screen across a cut between a wide shot and the close-up. An iris and a lit room use the two rules above, so the room does not move when an iris gives way to it.

## Textures: `.gtex`

A 20-byte header followed by pixel data, with every number a little-endian 32-bit unsigned integer.

| Offset | Meaning |
|---|---|
| 0 | The four letters `GTEX` |
| 4 | Width in pixels |
| 8 | Height in pixels |
| 12 | Format |
| 16 | Size of the pixel data in bytes |

| Format | Name | Pixel data size | Used for |
|---|---|---|---|
| 0 | RGBA8888 | width x height x 4 | Small images and tests |
| 1 | DXT5 | (width / 4) x (height / 4) x 16 | Art with transparency |
| 2 | DXT1 | (width / 4) x (height / 4) x 8 | Fully opaque art |

Width and height must both be multiples of 4 for DXT formats. Pixels run left to right, top to bottom, with straight (not premultiplied) alpha. Color under transparent pixels is filled from the nearest visible pixels, so bilinear filtering does not leave dark edges.

## Animations: `.anim`

```
animation st01_example_000
loops 1
frame st01_example_000_00 2 -40.5 -96 84 96 0 0 0.25 0.5
frame - 2 0 0 0 0 0 0 0 0
```

`frame` fields, in order:

| Field | Meaning |
|---|---|
| texture | Texture name, or `-` for a blank frame that draws nothing |
| ticks | How long the frame lasts, at 60 ticks per second |
| x, y | Top-left corner in DS pixels, relative to the object's position |
| width, height | Size in DS pixels |
| u, v | Top-left corner of the cell in the texture, from 0 to 1 |
| u_size, v_size | Size of the cell in the texture, from 0 to 1 |

An animation has at most 1024 frames.

The converter crops each atlas page to the cells the set actually uses, so `u` and `v` are measured against the cropped texture rather than the original page. A pose held alone on a page comes out as `0 0 1 1`.

## Scenes: `.scene`

A scene is a list of steps that run in order. Most take effect immediately. `wait`, `fade_in`, `fade_out` and `camera_pan` hold the script for the number of ticks they name, at 60 ticks per second, while animations keep playing. The audio steps, text steps and overlay steps never hold the script. Text fades and overlay growth run on their own timers, so the scene places a `wait` between them. They stop when a pause stops the scene, and they finish after the script ends. The end prompt waits until no text is still fading out.

`beam_animate`, `iris`, `dim_to` and `flash` do not hold the script. They run on their own timers while the script carries on with its next steps, and they stop when a pause stops the scene. Their ticks can be 0 to change at once. Positions are in DS pixels and pass through the same view as the art.

| Step | Meaning |
|---|---|
| `scene <name>` | Names the scene. Must be the first line |
| `room <room_id>` | Starts a new room: clears its plates and objects |
| `plate <texture> <x> <y> <width> <height>` | Adds a background plate to the room, in DS pixels. Later plates draw over earlier ones |
| `object <object_name> <animation> <x> <y> <layer> <flip>` | Adds an object, or changes the one with that name. Lower layers draw first. `flip` is 1 to mirror it horizontally, otherwise 0 |
| `remove_object <object_name>` | Removes an object |
| `camera <x> <y>` | Moves the camera's top-left corner, in DS pixels |
| `camera_pan <x> <y> <ticks>` | Moves the camera there in a straight line |
| `card <texture>` | Shows a 720 by 540 image, centred over black, over the room |
| `hide_card` | Removes the card |
| `text_show <id> <texture> <center_x> <top_y> <fade_ticks>` | Shows a text line at its texture's own size, in screen pixels, fading it in. Showing an `id` again replaces that line. Lines stay across rooms and are cleared when a scene starts or stops |
| `text_hide <id> <fade_ticks>` | Fades a line out from its current alpha, then removes it. 0 removes it at once |
| `text_hide_all` | Removes every line at once with no fade |
| `overlay <id> <texture> <x> <y> <width> <height> <grow_ticks>` | Shows an overlay, in DS pixels through the same view as objects, growing from nothing about its centre. Showing an `id` again replaces it. A new room clears overlays |
| `overlay_hide <id>` | Removes an overlay at once |
| `fade_out <ticks>` | Fades the screen to black |
| `fade_in <ticks>` | Fades the screen back in |
| `wait <ticks>` | Waits |
| `mask_on` | Everything outside the beams goes black across the whole screen. With no beams the screen is black |
| `mask_off` | Removes the mask and any iris |
| `beam_create <name> <apex_x> <apex_y> <base_x> <base_y> <half_width>` | Adds a beam: a triangle from its apex to a horizontal base, lit while the mask is on. Using a name again replaces that beam |
| `beam_animate <name> <base_x> <base_y> <half_width> <ticks>` | Moves a beam's base in a straight line. The apex stays |
| `beam_delete <name>` | Removes a beam |
| `iris <center_x> <center_y> <radius> <ticks>` | Opens a soft-edged circle in place of the mask, starting from nothing, or from its current size if an iris is already open. Black starts at `radius`, the picture is clear inside half of it, and nothing is drawn once the clear part reaches every screen corner |
| `dim_to <brightness> <ticks>` | Moves the picture's brightness, from 0 to 1, in a straight line. A full-screen black overlay does this |
| `flash <brightness> <ticks>` | A full-screen white overlay starting at that brightness and falling to nothing |
| `music <track> <loop>` | Plays a track, fading out any other first. `loop` is 1 to loop, otherwise 0 |
| `music_stop <ticks>` | Fades the music out over that many ticks. 0 stops it at once |
| `sound <sound>` | Starts a sound effect. Up to 4 play at once, and a fifth is logged and dropped |

A scene has at most 1024 steps and 4 beams at once.

Rooms do not change the mask or the beams. What draws over what, from the bottom: plates and objects, black around the art, the mask or iris, overlays, the dim, text lines, the flash, the card, the fade. So a bubble dims with the room and overflows a beam's edge, and text stays at full brightness above the dim.

A DS spotlight's `radius` becomes `half_width` as radius x 0.75 / 32, so 0x500 is 30 and 0xA00 is 60. The DS iris's radius 0x4000 is 384 by the same rule.

The preload waits for the first `music` track and every `sound` the scene names, and goes on if a file is missing or will not decode. A later `music` step loads its track when it runs. Replaying the scene, leaving it for browsing and quitting stop all audio, and the music also stops when the script ends.

## Text lines

Each line of narration becomes its own `.gtex` named `text_<message>_<line>`, in DXT5, white with its shape in alpha. The app has no fonts and draws these like any other texture. Bubble text works the same way, in its own color.

- Size is 38.5 screen pixels per em. A line is at most 840 pixels wide, and only a wider line is squeezed, by exactly enough to fit.
- Lines are centred on the 960 pixel screen. Every line of one size has the same texture height and baseline.
- The first line's ink starts at `2 + 0.75 x (8 + 3.77 x posY)` screen pixels, from the clip's 720p measurement scaled to the screen, plus the 2 pixels that centre the 540 pixel picture on the 544 pixel screen.
- Lines in a paragraph are 50.25 pixels apart. The next paragraph starts `59.25 x lines + 50.25` pixels below the first line of the one above.
- Bubble art and bubble text are placed to land on whole screen pixels, because the screen filters textures linearly and a fractional place would soften them. With the camera at `0 192` in a room 384 DS pixels tall, screen `x` is `2.8125 x` and screen `y` is `2.8125 x (y - 190.5778)`. Under the mask with no iris open the view stays centred on the DS window, so with the same camera screen `x` is `2.8125 x (x + 42.6667)` and screen `y` is `2.8125 x (y - 191.2889)`.

The set description names what to render, and its steps use these forms, which the converter expands into the steps above:

| Key or step | Meaning |
|---|---|
| `narration_font` | Pack path of the font for narration |
| `narration` | List of `name`, `file` and `entry` for each message |
| `bubble_text` | List of `name`, `file`, `entry`, `font`, `pixels_per_em` and `color` for one line of bubble text |
| `composed_textures` | List of `name`, `source`, `width`, `height` and `parts`, each part with `crop`, `x`, `y`, `width`, `height`, and optionally `flip_x` and `flip_y`, pasted in order over transparent white |
| `["narration_show", message, paragraph, fade_ticks]` | One `text_show` per line of that paragraph, with the line's place |
| `["narration_hide", message, fade_ticks]` | One `text_hide` per line of that message |
| `["overlay", id, texture, x, y, grow_ticks]` | One `overlay`, at the texture's own size on screen |

Nothing the converter writes holds story text as text. Decoded fonts and the working files for each line stay in a work folder in the system's temporary directory, never in the output folder. The converter deletes that folder when it finishes, fails, or receives SIGINT, SIGTERM or SIGHUP, even a second one while it is stopping. Only SIGKILL can leave it behind, still outside the repository, and the next conversion removes any such folder older than an hour. `tools/rom_audit.py` refuses fonts, message files, `.gtex` files, Ogg audio and DDS textures by name and by their first bytes, and `.rgba` work files by name.

Texture names must differ across atlas pages, plates, screen textures, composed textures, narration lines and bubble text, and by more than case, because each becomes one file. A set that repeats one is refused with a `failed:` line naming it, before anything is written.

Text and composed textures go through the same bake record as other textures, `.bake_record.json` in the output folder. A text line is baked again only when its decoded text, the stored bytes of its font, its size, width limit or color, or the code that draws it changes, so a repeat run decodes no font and draws nothing. A composed texture is baked again only when its source bytes, its description or its code changes. A message that only moves needs no new texture, because a line's place is part of the scene. The record holds hashes and each line's measured width. It holds no text and no font, and the text key is not part of any hash. A line or composed texture that fails is named on stderr with `failed:`, the rest still bake, and the last good scene and manifest stay.

## Manifest: `manifest.txt`

Lists what the viewer can browse and play, one per line.

```
scene opening
background st01_00
animation st01_example_000
```

## Audio: `.ogg` and `.loop`

`music/<track_name>.ogg` and `sounds/<sound_name>.ogg` are the remaster's own Ogg Vorbis files, unchanged apart from losing their wrapper. Every one must be 44100 Hz, and the converter and the app both refuse any other rate. Music plays on the BGM port and sounds on the VOICE port. Mono sounds play on both channels.

When the app opens an `.ogg` it first checks the CRC of every Ogg page, and the file must be whole pages from its first byte to its last. A bad CRC, a cut page or extra bytes after the last page log `decoder_failed` with `error=-1`, and the decoder never sees those bytes, because it can write out of bounds on damaged headers. This guards against damage only: a file built with correct CRCs and bad contents still reaches the decoder. A sound with no audio frames logs `decoder_failed` with `error=no_frames`.

`music/<track_name>.loop` exists only for a track whose wrapper sets its loop flag. It holds one line, the frame to go back to and the last frame to play:

```
1000 2000
```

A `music` step with `loop` 1 seeks to the first number after playing the frame named by the second. Without a `.loop` file it restarts at frame 0. The Ogg files' own `LoopStart` and `LoopEnd` tags are not read, because they say -1 on almost every stream.

## Source files

The byte offsets the converter reads from the remaster's files, and only those. For what the fields mean see [frame_mapping.md](../../docs/frame_mapping.md), and for the format codes and pack names see [inventory.md](../../docs/inventory.md). Numbers are little-endian.

`.asrc`, in `audio_file.py`, read only from the path under `natives/android/streaming/sound`, because the same name under `natives/android/sound` is a 16-byte stub:

| Offset | Size | Use |
|---|---|---|
| 0 | 4 | The letters `srcd` |
| 8 | 4 | Size of the Ogg file |
| 12 | 4 | The letters `ogg` and a space |
| 28 | 4 | Channels |
| 36 | 4 | Sample rate |
| 52 | 1 | Loop flag |
| 53 | 4 | Loop start frame, not aligned |
| 57 | 4 | Last frame, not aligned |
| 78 | 4 | Where the Ogg file starts, which must be 86 |
| 86 | | The Ogg file, starting with `OggS` |

`.tex`, in `texture_tool.c` and `texture_encoder.py`:

| Offset | Size | Use |
|---|---|---|
| 0 | 4 | The letters `TEX` and a zero byte |
| 8 | 2 | Width |
| 10 | 2 | Height |
| 12 | 2 | Must be 1. Meaning unknown |
| 14 | 1 | Must be 1. Meaning unknown |
| 15 | 1 | Mip count times 16 |
| 16 | 4 | Format code |
| 40 | 8 | Offset of the top mip's pixels |
| 52 | 4 | Size of the top mip's pixels |

`.user`, in `timeline_file.py`:

| Offset | Size | Use |
|---|---|---|
| 0 | 4 | The letters `USR` and a zero byte |
| 32 | 8 | Position of the RSZ block |
| RSZ + 0 | 4 | The letters `RSZ` and a zero byte |
| RSZ + 12 | 4 | Object count |
| RSZ + 24 | 8 | Offset of the object type table, from the RSZ block |
| RSZ + 32 | 8 | Offset of the object data, from the RSZ block |

A record holds 20 16-bit values. The converter reads values 0, 1, 2, 6 and 7. It calls 1 and 2 the anchor and 6 and 7 the point, because what they stand for is not established. The root's trailing values 0 and 2 are unknown and unused.

KPKA packs, in `pack_files.py`:

| Offset | Size | Use |
|---|---|---|
| 0 | 4 | The letters `KPKA` |
| 4 | 1 | Version, which must be 4 |
| 8 | 4 | Entry count |
| 16 | | The entry table, 48 bytes per entry |

An entry holds, in order, two 4-byte path hashes, then 8-byte values for the offset, stored size, plain size and attributes. The low 4 bits of the attributes are the compression method. The last 8 bytes are not read.

`.msg` version 22, in `message_file.py`:

| Offset | Size | Use |
|---|---|---|
| 0 | 4 | Version, which must be 22 |
| 4 | 4 | The letters `GMSG` |
| 16 | 4 | Entry count |
| 24 | 4 | Language count |
| 32 | 8 | Position of the string area |
| 72 | | One 8-byte position for each entry |
| entry + 24 | 8 | Position of the entry's name |
| entry + 40 | | One 8-byte position per language. Slot 1 is English |

The string area runs to the end of the file. Each byte is stored as `plain ^ previous stored byte ^ key[index % 16]`, and strings are UTF-16LE with a zero ending. Text holds tags of the form `<TRCK verb attribute="value" />`. The reader knows `mes_wait_set`, `mono_pos_set`, `paragraph_end`, `auto_pf` and `mono_type_set`, and refuses any other.

The 16-byte key is not in this repository. `tools/assets/extract_text_key.py` finds it in the library inside your own copy of the Android package and saves it as `text_key.bin` beside the packs. It never prints the key.

1. Entry names are ASCII, so every high byte in them is zero, and that gives the key bytes that fall on high bytes directly from the stored bytes of a message file in the packs.
2. A regular expression finds every 16-byte window of the library that has those bytes at those places.
3. A window counts as the key only if it decodes the whole string area so that every entry name holds only letters, digits and `_`, every string is strict UTF-16 and every string ends before the next one starts.

Exactly one distinct key must pass. No key, hash, position or expected byte is stored in the code. The converter reads `text_key.bin` only for a set with `narration` or `bubble_text`, and stops before any other work if the file is missing or the wrong size. The tool writes the key to a temporary `text_key_*.bin` file with owner-only permissions and renames it into place, so a stopped run leaves either the old file or a whole new one, and git ignores the temporary name as well.

`tools/rom_audit.py` refuses a file named `text_key.bin` and, by rule, any `.so` file or ELF library, because a library copied out of the package holds the key. When `extracted/remaster_apk/text_key.bin` exists, the audit also searches every tracked or unignored file for the key:

- any 8 bytes in a row of the key, in forward order, reverse order or with its 2-byte or 4-byte words swapped, as raw bytes, as a Python bytes literal, as C octal escapes, as hex text with anything between the bytes, or as decimal numbers with anything between them
- the whole key in base64, URL-safe base64, base32, base85 or as one big number
- the MD5, SHA-1, SHA-256 or SHA-512 digest of the key, as raw bytes or hex text

The audit also reads text saved as UTF-16. It reads each file's working copy and, when that differs, its staged copy, but never commit history, so a key that was committed and later removed is not found. It cannot find a compressed or encrypted copy of the key, or a piece shorter than 8 bytes. Without a key file it prints `key search skipped`, so a run in CI or a clean clone never looks for the key. It also cannot tell whether a key file holds the right bytes. Run it with the real key file in place before pushing.

`.oft`, in `font_file.py`:

| Offset | Size | Use |
|---|---|---|
| 0 | 4 | The letters `FBFO` |
| 4 | | An OpenType or TrueType font XORed with a repeating 8-byte key |

The key differs per file. The converter recovers it from the first 12 bytes of the font, whose values are fixed by the font format and its table count.

## Test audio

`tests/fixtures/sine_stereo_ogg.h` and `sine_mono_ogg.h` hold synthetic sine waves encoded with libvorbis at quality 0.1: 1 second of stereo at 440 and 660 Hz, and 0.25 seconds of mono at 880 Hz. They contain no game data. The stereo file's decoded CRC-32 is pinned in `tests/fixtures/fixture_facts.h`, and matched between arm64 and x86_64 builds with `-ffp-contract=off`. A test that edits a fixture and still wants it decoded calls `ogg_edit_refresh_crcs` from `tests/fixtures/ogg_edit.h` afterwards, because the app refuses a page whose CRC no longer matches.

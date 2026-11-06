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

An animation has at most 512 frames.

The converter crops each atlas page to the cells the set actually uses, so `u` and `v` are measured against the cropped texture rather than the original page. A pose held alone on a page comes out as `0 0 1 1`.

## Scenes: `.scene`

A scene is a list of steps that run in order. Most take effect immediately. `wait`, `fade_in`, `fade_out` and `camera_pan` hold the script for the number of ticks they name, at 60 ticks per second, while animations keep playing. The audio steps never hold the script.

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

Rooms do not change the mask or the beams. What draws over what, from the bottom: plates and objects, black around the art, the mask or iris, the dim, the flash, the card, the fade.

A DS spotlight's `radius` becomes `half_width` as radius x 0.75 / 32, so 0x500 is 30 and 0xA00 is 60. The DS iris's radius 0x4000 is 384 by the same rule.

The preload waits for the first `music` track and every `sound` the scene names, and goes on if a file is missing or will not decode. A later `music` step loads its track when it runs. Replaying the scene, leaving it for browsing and quitting stop all audio, and the music also stops when the script ends.

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

## Test audio

`tests/fixtures/sine_stereo_ogg.h` and `sine_mono_ogg.h` hold synthetic sine waves encoded with libvorbis at quality 0.1: 1 second of stereo at 440 and 660 Hz, and 0.25 seconds of mono at 880 Hz. They contain no game data. The stereo file's decoded CRC-32 is pinned in `tests/fixtures/fixture_facts.h`, and matched between arm64 and x86_64 builds with `-ffp-contract=off`. A test that edits a fixture and still wants it decoded calls `ogg_edit_refresh_crcs` from `tests/fixtures/ogg_edit.h` afterwards, because the app refuses a page whose CRC no longer matches.

# Converted asset format

The converter turns the remaster's Android files into small, simple files the Vita app can load. Everything it produces stays out of git, under `extracted/vita_assets/<set>/` on the Mac and `ux0:data/ghosttrick/assets/` on the Vita.

```
manifest.txt
textures/<texture_name>.gtex
animations/<animation_name>.anim
scenes/<scene_name>.scene
```

Names use only letters, digits, `_` and `-`, and are at most 95 characters. All text files use one command per line, with fields separated by a single space.

## Coordinates

The DS game logic works in DS pixels, so positions in these files do too. The remaster's art is 4 times the size of DS pixels.

The camera shows 256 by 192 DS pixels. On the Vita that view fills a playfield of 720 by 540 screen pixels, at x 120 and y 2, so one DS pixel is 2.8125 screen pixels. Art is converted to exactly that scale wherever possible, which means 0.703125 times its remaster size.

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

Width and height must both be multiples of 4 for DXT formats. Pixels run left to right, top to bottom, with straight (not premultiplied) alpha. Colour under transparent pixels is filled from the nearest visible pixels, so bilinear filtering does not leave dark edges.

## Animations: `.anim`

```
animation st01_example_000
ticks_per_second 60
loops 1
frame st01_example_000_00 2 -40.5 -96 84 96 0 0 0.25 0.5
frame - 2 0 0 0 0 0 0 0 0
```

`frame` fields, in order:

| Field | Meaning |
|---|---|
| texture | Texture name, or `-` for a blank frame that draws nothing |
| ticks | How long the frame lasts |
| x, y | Top-left corner in DS pixels, relative to the object's position |
| width, height | Size in DS pixels |
| u, v | Top-left corner of the cell in the texture, from 0 to 1 |
| u_size, v_size | Size of the cell in the texture, from 0 to 1 |

An animation has at most 512 frames.

## Scenes: `.scene`

A scene is a list of steps that run in order. Most take effect immediately. `wait`, `fade_in`, `fade_out` and `camera_pan` hold the script for the number of ticks they name, at 60 ticks per second, while animations keep playing.

| Step | Meaning |
|---|---|
| `scene <name>` | Names the scene. Must be the first line |
| `side_bars <left_texture> <right_texture>` | Art for the two bars beside the playfield |
| `room <room_id>` | Starts a new room: clears its plates and objects |
| `plate <texture> <x> <y> <width> <height>` | Adds a background plate to the room, in DS pixels. Later plates draw over earlier ones |
| `object <object_name> <animation> <x> <y> <layer> <flip>` | Adds an object, or changes the one with that name. Lower layers draw first. `flip` is 1 to mirror it horizontally, otherwise 0 |
| `remove_object <object_name>` | Removes an object |
| `camera <x> <y>` | Moves the camera's top-left corner, in DS pixels |
| `camera_pan <x> <y> <ticks>` | Moves the camera there in a straight line |
| `card <texture>` | Shows a full-playfield image over the room |
| `hide_card` | Removes the card |
| `fade_out <ticks>` | Fades the playfield to black |
| `fade_in <ticks>` | Fades the playfield back in |
| `wait <ticks>` | Waits |

A scene has at most 1024 steps.

## Manifest: `manifest.txt`

Lists what the viewer can browse and play, one per line.

```
scene opening
background st01_00
animation st01_example_000
```

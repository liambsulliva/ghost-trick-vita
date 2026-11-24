#!/bin/sh
# Draws the release package's LiveArea icon and banner from the Twemoji fire emoji (U+1F525),
# kept unchanged as twemoji-1f525.svg and credited in NOTICE. The outer flame is recoloured blue
# and the core a paler blue; the shapes are the emoji's own. Both images sit on a near-black
# backdrop. The icon is the flame alone; the banner puts a smaller flame on the left, the title on
# one line on the right, and a small line of sentence-case text under it. The banner is drawn at
# four times its size and shrunk, so its text has smooth edges. The subtitle is DejaVu Sans with a
# hairline outline in its own colour, standing in for a semibold face, since none is installed.
# ImageMagick draws the SVG's path data itself, so no SVG renderer is needed. Run it again after
# changing the colours and commit the PNGs it writes.
set -eu
folder=$(cd "$(dirname "$0")" && pwd)
source="$folder/twemoji-1f525.svg"
backdrop='#0E0E0E'
letters='#EBF0FA'
vita='#2C89E3'
outer='#2E8BE6'
core='#9FD4FF'
font=DejaVu-Sans-Bold
subfont=DejaVu-Sans
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

outer_path=$(sed -n 's/.*fill="#F4900C" d="\([^"]*\)".*/\1/p' "$source")
core_path=$(sed -n 's/.*fill="#FFCC4D" d="\([^"]*\)".*/\1/p' "$source")

# The emoji's 36-unit artboard drawn at 4 pixels a unit, then scaled to 112 pixels.
magick -size 144x144 xc:none \
    -fill "$outer" -draw "scale 4,4 path '$outer_path'" \
    -fill "$core" -draw "scale 4,4 path '$core_path'" \
    -resize 112x112 "$work/fire.png"

magick -size 128x128 "xc:$backdrop" "$work/fire.png" -geometry +8+8 -composite \
    -colors 256 -define png:exclude-chunks=date,time "PNG8:$folder/icon0.png"

# The banner at four times its size: the flame at 352 pixels, the title at 88 points and the
# subtitle at 40 points, with the subtitle's outline and tracking scaled to match. Shrinking it
# to 280 by 158 with a box filter gives the final pixels.
magick -size 1120x632 "xc:$backdrop" \( "$folder/icon0.png" -resize 352x352 \) -geometry +72+140 -composite \
    -font "$font" -fill "$letters" -pointsize 88 -annotate +472+312 'Ghost Trick' \
    -font "$subfont" -fill "$vita" -stroke "$vita" -strokewidth 1.4 -kerning 2.5 -pointsize 40 -annotate +472+388 'For PlayStation Vita' -stroke none \
    -filter Box -resize 280x158 \
    -colors 256 -define png:exclude-chunks=date,time "PNG8:$folder/startup.png"

#!/bin/sh
# Draws the release package's LiveArea icon and banner from shapes written here, not from any
# game image. The flame is a pear-shaped body, wide at the bottom and narrowing to a rounded
# top, with two small wisps on its crown and a pale orb set low in its middle. It is filled with
# a gradient and a soft glow, on charcoal. The banner puts the flame on the left, the title on one
# line on the right and VITA under it. Run it again after changing the drawing and commit the PNGs
# it writes.
set -eu
folder=$(cd "$(dirname "$0")" && pwd)
charcoal='#36454F'
letters='#EBF0FA'
teal='#40DCBF'
orb='#D6DBF5'
font=DejaVu-Sans-Bold
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

# The body and its two wisps as a white mask on black.
magick -size 128x128 xc:black -fill white \
    -draw 'path "M 64,16 C 72,16 76,24 76,32 C 80,44 88,52 96,68 C 102,80 100,104 90,112 C 82,118 72,120 64,120 C 56,120 46,118 38,112 C 28,104 26,80 32,68 C 40,52 48,44 52,32 C 52,24 56,16 64,16 Z"' \
    -draw 'circle 56,22 62,22' \
    -draw 'circle 74,21 82,21' \
    -blur 0x0.5 "$work/mask.png"

# The gradient runs from near-white at the crown to pale blue at the base, cut out by the mask.
magick -size 128x128 'gradient:#FFFFFF-#8FD3F4' "$work/mask.png" \
    -alpha off -compose CopyOpacity -composite "$work/flame.png"

# The orb: a soft pale disc with a white heart, set low in the body.
magick -size 128x128 xc:none -fill "$orb" -draw 'circle 64,82 64,98' -blur 0x2 "$work/orb_outer.png"
magick -size 128x128 xc:none -fill "#FFFFFF" -draw 'circle 64,82 64,90' -blur 0x1.5 "$work/orb_heart.png"

# A soft glow around the flame.
magick "$work/flame.png" -channel A -evaluate multiply 0.40 +channel -blur 0x6 "$work/glow.png"

magick -size 128x128 "xc:$charcoal" \
    "$work/glow.png" -composite "$work/flame.png" -composite \
    "$work/orb_outer.png" -composite "$work/orb_heart.png" -composite \
    -colors 256 -define png:exclude-chunks=date,time "PNG8:$folder/icon0.png"

magick "$folder/icon0.png" -resize 112x112 "$work/icon112.png"

magick -size 280x158 "xc:$charcoal" "$work/icon112.png" -geometry +14+23 -composite \
    -font "$font" -fill "$letters" -pointsize 19 -annotate +142+72 'Ghost Trick' \
    -fill "$teal" -pointsize 16 -annotate +142+100 'VITA' \
    -colors 256 -define png:exclude-chunks=date,time "PNG8:$folder/startup.png"

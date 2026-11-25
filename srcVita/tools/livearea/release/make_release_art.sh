#!/bin/sh
# Draws the release package's LiveArea art from the Twemoji fire emoji (U+1F525), kept unchanged as
# twemoji-1f525.svg and credited in NOTICE. The outer flame is recoloured blue and the core a paler
# blue; the shapes are the emoji's own. Everything sits on a near-black backdrop.
#   icon0.png: the flame alone, with clear padding for the Vita's round bubble.
#   startup.png: the banner, with a smaller flame on the left, the title on one line on the right and
#     a small line of sentence-case text under it. It is drawn at four times its size and shrunk, so
#     its text has smooth edges. The subtitle is DejaVu Sans with a hairline outline in its own colour,
#     standing in for a semibold face, since none is installed.
#   bg.png: a large flame on the backdrop, with no text, since the gate already shows the title.
#   readme-banner.png: the banner's layout at eight times its size, for the README. Its flame is drawn from
#     the SVG paths at that size, so its edges stay sharp.
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

# The icon: the flame at 84 pixels, centred on the 128 pixel backdrop, so the bubble leaves it room.
magick -size 128x128 "xc:$backdrop" \( "$work/fire.png" -resize 84x84 \) -geometry +22+22 -composite \
    -colors 256 -define png:exclude-chunks=date,time "PNG8:$folder/icon0.png"

# The banner's flame: 308 pixels on a 352 pixel square, drawn at four times the banner's size.
magick -size 352x352 "xc:$backdrop" \( "$work/fire.png" -resize 308x308 \) -geometry +22+22 -composite "$work/logo352.png"

magick -size 1120x632 "xc:$backdrop" "$work/logo352.png" -geometry +72+140 -composite \
    -font "$font" -fill "$letters" -pointsize 88 -annotate +472+312 'Ghost Trick' \
    -font "$subfont" -fill "$vita" -stroke "$vita" -strokewidth 1.4 -kerning 2.5 -pointsize 40 -annotate +472+388 'For PlayStation Vita' -stroke none \
    "$work/banner4x.png"

magick "$work/banner4x.png" -filter Box -resize 280x158 \
    -colors 256 -define png:exclude-chunks=date,time "PNG8:$folder/startup.png"

# The background: the flame, large, in the middle of the 840 by 500 backdrop.
magick -size 840x500 "xc:$backdrop" \( "$work/fire.png" -resize 300x300 \) -geometry +270+100 -composite \
    -colors 256 -define png:exclude-chunks=date,time "PNG8:$folder/bg.png"

# The README banner: the banner's layout at eight times its size. The flame is the SVG paths drawn at
# that size, not the 112 pixel file above, so it is not scaled up from a smaller picture.
magick -size 2240x1264 "xc:$backdrop" \
    -fill "$outer" -draw "translate 188,324 scale 17.1111,17.1111 path '$outer_path'" \
    -fill "$core" -draw "translate 188,324 scale 17.1111,17.1111 path '$core_path'" \
    -font "$font" -fill "$letters" -pointsize 176 -annotate +944+624 'Ghost Trick' \
    -font "$subfont" -fill "$vita" -stroke "$vita" -strokewidth 2.8 -kerning 5 -pointsize 80 -annotate +944+776 'For PlayStation Vita' -stroke none \
    -colors 256 -define png:exclude-chunks=date,time "PNG8:$folder/readme-banner.png"

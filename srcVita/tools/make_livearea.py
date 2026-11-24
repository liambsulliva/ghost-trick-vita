import argparse
import os
import subprocess
import sys
import tempfile

TOOLS_FOLDER = os.path.dirname(os.path.abspath(__file__))
SRC_VITA_FOLDER = os.path.dirname(TOOLS_FOLDER)
ASSETS_FOLDER = os.path.join(os.path.dirname(SRC_VITA_FOLDER), "extracted", "remaster_apk")
TEXTURE_TOOL = os.path.join(SRC_VITA_FOLDER, "build", "tools", "texture_tool")
DS_ROM_PATH = os.path.join(os.path.dirname(SRC_VITA_FOLDER), "extracted", "ds_rom", "game.nds")
GAME_DESCRIPTION = os.path.join(TOOLS_FOLDER, "assets", "game.json")
DS_LOGO_NAME = "title_logo"
DS_BACKDROP_NAME = "pause_backdrop"
LOGO_PACK_PATH = "natives/android/charfiles/3d/06_id/11_title/title_up_iml4.tex.691230823.en"
FIELD_COLOR = "#0c1523"
RELEASE_ART_FOLDER = os.path.join(TOOLS_FOLDER, "livearea", "release")
LOGO_LAYOUTS = (
    ("icon0.png", 128, 128, 120, 120),
    ("livearea/contents/bg.png", 840, 500, 600, 300),
    ("livearea/contents/startup.png", 280, 158, 250, 140),
)

sys.path.insert(0, os.path.join(TOOLS_FOLDER, "assets"))

import pack_files
import texture_encoder

def write_file(path, data):
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "wb") as output:
        output.write(data)


def assets_available(assets_folder, texture_tool):
    return os.path.isdir(assets_folder) and os.path.isfile(texture_tool)


def export_logo(assets_folder, texture_tool, work_folder):
    tex_bytes = pack_files.PackSet(assets_folder).read(LOGO_PACK_PATH)
    width, height = texture_encoder.read_tex_size(tex_bytes)
    tex_path = os.path.join(work_folder, "logo.tex")
    rgba_path = os.path.join(work_folder, "logo.rgba")
    write_file(tex_path, tex_bytes)
    texture_encoder.run_texture_tool(texture_tool, tex_path, rgba_path, width, height)
    return rgba_path, width, height


def compose_command(logo_path, logo_width, logo_height, layout, output_path):
    _, canvas_width, canvas_height, box_width, box_height = layout
    return [
        "magick", "-size", "%dx%d" % (logo_width, logo_height), "-depth", "8", "rgba:" + logo_path,
        "-background", FIELD_COLOR, "-flatten", "-trim", "+repage",
        "-resize", "%dx%d" % (box_width, box_height),
        "-gravity", "center", "-extent", "%dx%d" % (canvas_width, canvas_height),
        "-colors", "256", "-define", "png:exclude-chunks=date,time", "PNG8:" + output_path,
    ]


def write_logo_images(output_folder, assets_folder, texture_tool):
    with tempfile.TemporaryDirectory() as work_folder:
        logo_path, logo_width, logo_height = export_logo(assets_folder, texture_tool, work_folder)
        for layout in LOGO_LAYOUTS:
            output_path = os.path.join(output_folder, layout[0])
            os.makedirs(os.path.dirname(output_path), exist_ok=True)
            subprocess.run(compose_command(logo_path, logo_width, logo_height, layout, output_path), check=True)


def ds_logo_and_backdrop(rom_path):
    import json
    import ds_set
    import ds_ui
    with open(GAME_DESCRIPTION, encoding="utf-8") as source:
        textures = {entry["name"]: entry for entry in json.load(source)["ds_textures"]}
    cartridge = ds_set.Cartridge(rom_path)
    texture_of = lambda index, opaque: cartridge.textures.texture(index, 1, not opaque)
    return ds_ui.compose(textures[DS_LOGO_NAME], texture_of), ds_ui.compose(textures[DS_BACKDROP_NAME], texture_of)


def ds_compose_command(logo_path, logo_size, backdrop_path, backdrop_size, layout, output_path):
    _, canvas_width, canvas_height, box_width, box_height = layout
    return [
        "magick",
        "(", "-size", "%dx%d" % backdrop_size, "-depth", "8", "rgba:" + backdrop_path,
        "-filter", "point", "-resize", "%dx%d^" % (canvas_width, canvas_height),
        "-gravity", "center", "-extent", "%dx%d" % (canvas_width, canvas_height), ")",
        "(", "-size", "%dx%d" % logo_size, "-depth", "8", "rgba:" + logo_path,
        "-trim", "+repage", "-filter", "point", "-resize", "%dx%d" % (box_width, box_height), ")",
        "-gravity", "center", "-composite", "-alpha", "off",
        "-colors", "256", "-define", "png:exclude-chunks=date,time", "PNG8:" + output_path,
    ]


def write_ds_logo_images(output_folder, rom_path):
    logo, backdrop = ds_logo_and_backdrop(rom_path)
    with tempfile.TemporaryDirectory() as work_folder:
        logo_path = os.path.join(work_folder, "logo.rgba")
        backdrop_path = os.path.join(work_folder, "backdrop.rgba")
        write_file(logo_path, logo[2])
        write_file(backdrop_path, backdrop[2])
        for layout in LOGO_LAYOUTS:
            output_path = os.path.join(output_folder, layout[0])
            os.makedirs(os.path.dirname(output_path), exist_ok=True)
            subprocess.run(ds_compose_command(logo_path, logo[:2], backdrop_path, backdrop[:2], layout, output_path), check=True)


def read_file(path):
    with open(path, "rb") as source:
        return source.read()


def write_monogram_images(output_folder):
    # The icon, background and banner are the release art in tools/livearea/release/ (its NOTICE has the
    # credit), which holds no game art, so a package built without the game's files gets them.
    write_file(os.path.join(output_folder, "icon0.png"), read_file(os.path.join(RELEASE_ART_FOLDER, "icon0.png")))
    write_file(os.path.join(output_folder, "livearea/contents/bg.png"), read_file(os.path.join(RELEASE_ART_FOLDER, "bg.png")))
    write_file(os.path.join(output_folder, "livearea/contents/startup.png"), read_file(os.path.join(RELEASE_ART_FOLDER, "startup.png")))


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("output_folder")
    parser.add_argument("--assets-folder", default=ASSETS_FOLDER)
    parser.add_argument("--texture-tool", default=TEXTURE_TOOL)
    parser.add_argument("--ds-rom", default=DS_ROM_PATH)
    parser.add_argument("--monogram", action="store_true", help="use the release art even when the extracted assets are present, as release packages do")
    arguments = parser.parse_args()
    if arguments.monogram:
        write_monogram_images(arguments.output_folder)
        return 0
    if assets_available(arguments.assets_folder, arguments.texture_tool):
        write_logo_images(arguments.output_folder, arguments.assets_folder, arguments.texture_tool)
    elif os.path.isfile(arguments.ds_rom):
        print("make_livearea: no remaster assets, using the DS cartridge's title logo", file=sys.stderr)
        write_ds_logo_images(arguments.output_folder, arguments.ds_rom)
    else:
        print("make_livearea: game assets or texture_tool missing, using the release art", file=sys.stderr)
        write_monogram_images(arguments.output_folder)
    return 0


if __name__ == "__main__":
    sys.exit(main())

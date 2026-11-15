import os
import re
import unittest

SRC_VITA_FOLDER = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

LIBRARY_OF_HEADER = {
    "psp2/appmgr.h": "SceAppMgr_stub",
    "psp2/apputil.h": "SceAppUtil_stub",
    "psp2/audioout.h": "SceAudio_stub",
    "psp2/ctrl.h": "SceCtrl_stub",
    "psp2/display.h": "SceDisplay_stub",
    "psp2/gxm.h": "SceGxm_stub",
    "psp2/power.h": "ScePower_stub",
    "psp2/touch.h": "SceTouch_stub",
    "vitaGL.h": "vitaGL",
}
LINKED_BY_DEFAULT = ("psp2/io/", "psp2/kernel/", "psp2/system_param.h", "psp2/types.h")


def included_sdk_headers():
    headers = set()
    for folder, _, file_names in os.walk(os.path.join(SRC_VITA_FOLDER, "src")):
        for file_name in file_names:
            if file_name.endswith(".c"):
                with open(os.path.join(folder, file_name), encoding="utf-8", errors="replace") as source:
                    headers.update(re.findall(r"#include <((?:psp2/[^>]+)|vitaGL\.h)>", source.read()))
    return headers


def linked_libraries():
    with open(os.path.join(SRC_VITA_FOLDER, "Makefile"), encoding="utf-8") as source:
        text = source.read()
    block = re.search(r"^LIBS\s*:=((?:.*\\\n)*.*)$", text, re.M).group(1)
    return set(re.findall(r"-l(\S+)", block))


class EverySdkHeaderHasItsLibrary(unittest.TestCase):
    def test_each_included_sdk_header_is_known(self):
        unknown = sorted(header for header in included_sdk_headers() if header not in LIBRARY_OF_HEADER and not header.startswith(LINKED_BY_DEFAULT))
        self.assertEqual(unknown, [])

    def test_each_included_sdk_header_has_its_library_on_the_link_line(self):
        libraries = linked_libraries()
        missing = sorted("%s needs -l%s" % (header, LIBRARY_OF_HEADER[header]) for header in included_sdk_headers() if header in LIBRARY_OF_HEADER and LIBRARY_OF_HEADER[header] not in libraries)
        self.assertEqual(missing, [])


if __name__ == "__main__":
    unittest.main()

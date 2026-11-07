import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", "tools"))

import coverage_report


class CoverageReportTests(unittest.TestCase):
    def test_counts_each_missing_thing_once_per_name(self):
        lines = coverage_report.report_lines([
            "mark op_not_ported  op=0xdf file=a object=b tick=1\n",
            "mark op_not_ported  op=0xdf file=a object=b tick=2\n",
            "mark gameapp_not_ported Timeleap|OTHER op=0x12 file=a\n",
            "mark route_step_done 1:move:a:0\n",
        ])
        self.assertEqual(lines, ["gameapp_not_ported 0x12 x1", "op_not_ported 0xdf x2"])


if __name__ == "__main__":
    unittest.main()

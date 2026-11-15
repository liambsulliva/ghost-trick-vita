import struct
import unittest

import arm64_runner

CODE_BASE = 0x10000
STUB_ADDRESS = CODE_BASE + 0x1000


class FakeLibrary:
    def __init__(self, words, names=None):
        self.words = words
        self.names = names or {}
        self.address_of = {}
        self.external_names = {}

    def instruction(self, address):
        index = (address - CODE_BASE) // 4
        if address < CODE_BASE or index >= len(self.words):
            return None
        return self.words[index]

    def page(self, base):
        return None

    def name_of_call(self, address):
        return self.names.get(address)


def run(words, stubs=None, names=None, registers=None):
    machine = arm64_runner.Machine(FakeLibrary(words + [0xd65f03c0], names), stubs or {})
    machine.call(CODE_BASE, registers or [])
    return machine


class IntegerTests(unittest.TestCase):
    def test_moves_adds_selects_and_bitfields(self):
        machine = run([
            0xd2824680, 0xf2aacf00, 0x12800001, 0x32089fe2, 0x91004003, 0xd1408064,
            0x6b020045, 0x9a830006, 0xeb03001f, 0x9a832407, 0x53003c48, 0x531c6d09, 0x93483c4a,
        ])
        x = machine.x
        self.assertEqual(x[0], 0x56781234)
        self.assertEqual(x[1], 0xffffffff)
        self.assertEqual(x[2], 0xff00ff00)
        self.assertEqual(x[3], 0x56781244)
        self.assertEqual(x[4], 0x56761244)
        self.assertEqual(x[5], 0)
        self.assertEqual(x[6], 0x56781234)
        self.assertEqual(x[7], 0x56781245)
        self.assertEqual(x[8], 0xff00)
        self.assertEqual(x[9], 0xff000)
        self.assertEqual(x[10], arm64_runner.MASK64)

    def test_loads_stores_with_write_back_and_exclusives(self):
        machine = run([
            0xd2824680, 0xf2aacf00, 0x91004003,
            0xf81f0fe0, 0xf84107eb, 0x79000460, 0x7940046c, 0x3980086d,
            0xa9be0fe0, 0xa8c23fee, 0xc85ffc70, 0xc811fc60,
        ])
        x = machine.x
        self.assertEqual(x[11], 0x56781234)
        self.assertEqual(x[12], 0x1234)
        self.assertEqual(x[13], 0x34)
        self.assertEqual((x[14], x[15]), (0x56781234, 0x56781244))
        self.assertEqual(x[16], 0x12340000)
        self.assertEqual(x[17], 0)
        self.assertEqual(machine.memory.u(0x56781244, 8), 0x56781234)
        self.assertEqual(x[31], arm64_runner.STACK_TOP)

    def test_multiply_divide_and_variable_shift(self):
        machine = run([0xd2824680, 0x91004003, 0xd1408064, 0x531c6d09, 0x9b031014, 0x9ac30815, 0x1ac92016])
        x = machine.x
        self.assertEqual(x[20], (x[4] + x[0] * x[3]) & arm64_runner.MASK64)
        self.assertEqual(x[21], 0)
        self.assertEqual(x[22], (x[0] << (x[9] % 32)) & arm64_runner.MASK32)


class FloatTests(unittest.TestCase):
    def test_convert_multiply_compare_and_vector_zero(self):
        machine = run([0xd2800060, 0x1e220000, 0x1e201001, 0x1e210802, 0x1e380052, 0x6f00e403, 0x1e7f1004, 0x1e602088, 0x1a9fa7f3])
        self.assertEqual(machine.x[18], 6)
        self.assertEqual(machine.v[3], 0)
        self.assertEqual(arm64_runner.unpack_float(machine.v[4], True), -1.5)
        self.assertEqual(machine.x[19], 1)

    def test_float_immediates(self):
        self.assertEqual(arm64_runner.unpack_float(arm64_runner.float_immediate(0x70, False), False), 1.0)
        self.assertEqual(arm64_runner.unpack_float(arm64_runner.float_immediate(0x00, False), False), 2.0)
        self.assertEqual(arm64_runner.unpack_float(arm64_runner.float_immediate(0x60, True), True), 0.5)


class BranchTests(unittest.TestCase):
    def test_conditional_branches_skip_their_targets(self):
        machine = run([
            0xd2800000, 0xb4000040, 0xd2800021, 0x54000041, 0xd2800042,
            0xd2800083, 0x37100043, 0xd28000a4, 0xd28000c5,
        ])
        x = machine.x
        self.assertEqual(x[1], 0)
        self.assertEqual(x[2], 0)
        self.assertEqual(x[3], 4)
        self.assertEqual(x[4], 0)
        self.assertEqual(x[5], 6)

    def test_a_named_call_runs_its_stub_and_returns(self):
        calls = []

        def stub(machine):
            calls.append(machine.x[0])
            return 99
        words = [0xd2800540, 0x94000000 | (STUB_ADDRESS - CODE_BASE - 4) // 4, 0xaa0003e9]
        machine = run(words, {"stubbed": stub}, {STUB_ADDRESS: "stubbed"})
        self.assertEqual(calls, [42])
        self.assertEqual(machine.x[9], 99)

    def test_a_call_with_no_stub_names_the_function(self):
        words = [0x94000000 | (STUB_ADDRESS - CODE_BASE) // 4]
        with self.assertRaisesRegex(arm64_runner.RunError, "missing_function with no stub"):
            run(words, {}, {STUB_ADDRESS: "missing_function"})

    def test_an_indirect_call_hook_can_stop_the_run(self):
        def stop(machine, target):
            raise arm64_runner.StopRun()
        machine = arm64_runner.Machine(FakeLibrary([0xd2800085, 0xd63f00a0, 0xd2800120, 0xd65f03c0]), {}, on_indirect_call=stop)
        self.assertIsNone(machine.call(CODE_BASE))
        self.assertEqual(machine.x[5], 4)
        self.assertNotEqual(machine.x[0], 9)

    def test_an_unknown_instruction_is_refused_with_its_address(self):
        with self.assertRaisesRegex(arm64_runner.RunError, hex(CODE_BASE)):
            run([0x00000000])


class MaskTests(unittest.TestCase):
    def test_bit_masks(self):
        self.assertEqual(arm64_runner.decode_bit_masks(0, 0b000111, 0, 32)[0], 0xff)
        self.assertEqual(arm64_runner.decode_bit_masks(1, 0b111110, 0, 64)[0], (1 << 63) - 1)

    def test_vector_immediates(self):
        self.assertEqual(arm64_runner.simd_immediate(0x6f00e403), 0)
        self.assertEqual(arm64_runner.simd_immediate(0x0f0087e0), 0x001f001f001f001f)


if __name__ == "__main__":
    unittest.main()

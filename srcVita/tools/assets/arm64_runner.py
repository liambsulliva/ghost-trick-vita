import math
import struct

MASK64 = (1 << 64) - 1
MASK32 = (1 << 32) - 1
PAGE = 0x1000
RELOCATION_SIZE = 24
SYMBOL_SIZE = 24
R_ABS64 = 257
R_GLOB_DAT = 1025
R_JUMP_SLOT = 1026
R_RELATIVE = 1027
SECTION_HEADER_SIZE = 64
PROGRAM_HEADER_SIZE = 56
PT_LOAD = 1
EXTERNAL_BASE = 0x7e0000000000
EXTERNAL_STRIDE = 0x100
HEAP_BASE = 0x700000000000
STACK_TOP = 0x7f0000100000
STACK_SIZE = 0x400000
RETURN_SENTINEL = 0x7ffffffff000
THREAD_BLOCK = 0x7f1000000000
UNBOUND_SLOT = "<unbound>"


class RunError(RuntimeError):
    pass


class StopRun(Exception):
    pass


class Library:
    def __init__(self, data):
        self.data = data
        if data[:4] != b"\x7fELF" or data[4] != 2:
            raise RunError("the library is not a 64-bit ELF file")
        program_offset, section_offset = struct.unpack_from("<QQ", data, 0x20)
        program_count = struct.unpack_from("<H", data, 0x38)[0]
        section_count, names_index = struct.unpack_from("<HH", data, 0x3c)
        self.loads = []
        for index in range(program_count):
            kind, _, offset, address, _, file_size, memory_size = struct.unpack_from("<IIQQQQQ", data, program_offset + PROGRAM_HEADER_SIZE * index)
            if kind == PT_LOAD:
                self.loads.append((address, address + memory_size, offset, file_size))
        sections = []
        for index in range(section_count):
            name, kind, _, address, offset, size, link = struct.unpack_from("<IIQQQQI", data, section_offset + SECTION_HEADER_SIZE * index)
            sections.append((name, kind, address, offset, size, link))
        names_offset = sections[names_index][3]
        self.sections = {}
        for name, kind, address, offset, size, link in sections:
            end = data.index(b"\0", names_offset + name)
            self.sections[data[names_offset + name:end].decode("ascii")] = (address, offset, size)
        self.read_symbols()
        self.read_relocations()

    def section(self, name):
        if name not in self.sections:
            raise RunError("the library has no " + name + " section")
        return self.sections[name]

    def read_symbols(self):
        _, symbol_offset, symbol_size = self.section(".dynsym")
        _, string_offset, _ = self.section(".dynstr")
        self.symbol_names = []
        self.symbol_values = []
        self.address_of = {}
        self.size_of = {}
        for index in range(symbol_size // SYMBOL_SIZE):
            name, _, _, _, value, size = struct.unpack_from("<IBBHQQ", self.data, symbol_offset + SYMBOL_SIZE * index)
            start = string_offset + name
            text = self.data[start:self.data.index(b"\0", start)].decode("ascii", "replace")
            self.symbol_names.append(text)
            self.symbol_values.append(value)
            if value:
                self.address_of[text] = value
                self.size_of[text] = size

    def read_relocations(self):
        self.relocated = {}
        self.slot_names = {}
        self.external_names = {}
        external_addresses = {}
        for section_name in (".rela.dyn", ".rela.plt"):
            _, offset, size = self.section(section_name)
            for position in range(offset, offset + size, RELOCATION_SIZE):
                target, info, addend = struct.unpack_from("<QQq", self.data, position)
                kind = info & MASK32
                symbol = info >> 32
                if kind == R_RELATIVE:
                    self.relocated[target] = addend & MASK64
                    continue
                if kind not in (R_ABS64, R_GLOB_DAT, R_JUMP_SLOT):
                    continue
                name = self.symbol_names[symbol]
                value = self.symbol_values[symbol]
                if kind == R_JUMP_SLOT:
                    self.slot_names[target] = name
                if not value:
                    if name not in external_addresses:
                        external_addresses[name] = EXTERNAL_BASE + EXTERNAL_STRIDE * len(external_addresses)
                        self.external_names[external_addresses[name]] = name
                    value = external_addresses[name]
                self.relocated[target] = (value + addend) & MASK64

    def file_bytes(self, address, size):
        for start, end, offset, file_size in self.loads:
            if start <= address < end:
                inside = address - start
                available = max(0, min(size, file_size - inside))
                return self.data[offset + inside:offset + inside + available] + b"\0" * (size - available)
        return None

    def page(self, base):
        raw = self.file_bytes(base, PAGE)
        if raw is None:
            return None
        page = bytearray(raw)
        for slot in range(base, base + PAGE, 8):
            if slot in self.relocated:
                struct.pack_into("<Q", page, slot - base, self.relocated[slot])
        return page

    def name_of_call(self, address):
        if address in self.external_names:
            return self.external_names[address]
        word = self.instruction(address)
        if word is None or (word & 0x9f00001f) != 0x90000010:
            return None
        page = adrp_target(address, word)
        load = self.instruction(address + 4)
        if load is None or (load & 0xffc003ff) != 0xf9400211:
            return None
        return self.slot_names.get(page + ((load >> 10) & 0xfff) * 8, UNBOUND_SLOT)

    def instruction(self, address):
        raw = self.file_bytes(address, 4)
        return None if raw is None else struct.unpack("<I", raw)[0]


class Memory:
    def __init__(self, library):
        self.library = library
        self.pages = {}

    def page_of(self, address):
        base = address & ~(PAGE - 1)
        page = self.pages.get(base)
        if page is None:
            page = self.library.page(base) or bytearray(PAGE)
            self.pages[base] = page
        return page, address - base

    def read(self, address, size):
        out = bytearray()
        while size:
            page, inside = self.page_of(address)
            take = min(size, PAGE - inside)
            out += page[inside:inside + take]
            address += take
            size -= take
        return bytes(out)

    def write(self, address, raw):
        position = 0
        while position < len(raw):
            page, inside = self.page_of(address + position)
            take = min(len(raw) - position, PAGE - inside)
            page[inside:inside + take] = raw[position:position + take]
            position += take

    def u(self, address, size):
        page, inside = self.page_of(address)
        if inside + size <= PAGE:
            return int.from_bytes(page[inside:inside + size], "little")
        return int.from_bytes(self.read(address, size), "little")

    def put(self, address, size, value):
        page, inside = self.page_of(address)
        if inside + size <= PAGE:
            page[inside:inside + size] = (value & ((1 << (8 * size)) - 1)).to_bytes(size, "little")
        else:
            self.write(address, (value & ((1 << (8 * size)) - 1)).to_bytes(size, "little"))


def adrp_target(address, word):
    immediate = ((word >> 29) & 3) | (((word >> 5) & 0x7ffff) << 2)
    if immediate & (1 << 20):
        immediate -= 1 << 21
    return ((address & ~0xfff) + (immediate << 12)) & MASK64


def sign_extend(value, bits):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def ror(value, amount, width):
    amount %= width
    return ((value >> amount) | (value << (width - amount))) & ((1 << width) - 1)


def decode_bit_masks(n, imms, immr, width):
    length = (n << 6 | (~imms & 0x3f)).bit_length() - 1
    if length < 1:
        raise RunError("reserved bitmask immediate")
    size = 1 << length
    levels = size - 1
    s = imms & levels
    r = immr & levels
    element = ror((1 << (s + 1)) - 1, r, size)
    wmask = 0
    for _ in range(width // size):
        wmask = (wmask << size) | element
    top = (1 << ((s - r) & levels) + 1) - 1
    tmask = 0
    for _ in range(width // size):
        tmask = (tmask << size) | top
    return wmask, tmask


class Machine:
    def __init__(self, library, stubs, may_run=None, on_indirect_call=None):
        self.library = library
        self.may_run = may_run or (lambda name: True)
        self.on_indirect_call = on_indirect_call
        self.memory = Memory(library)
        self.stubs = stubs
        self.x = [0] * 32
        self.v = [0] * 32
        self.n = self.z = self.c = self.v_flag = 0
        self.heap = HEAP_BASE
        self.executed = 0
        self.call_names = {}

    def allocate(self, size):
        address = self.heap
        self.heap += (size + 0xf) & ~0xf
        return address

    def reg(self, index, wide=True):
        if index == 31:
            return 0
        return self.x[index] if wide else self.x[index] & MASK32

    def reg_or_sp(self, index, wide=True):
        value = self.x[31] if index == 31 else self.x[index]
        return value if wide else value & MASK32

    def set_reg(self, index, value, wide=True):
        if index != 31:
            self.x[index] = value & (MASK64 if wide else MASK32)

    def set_reg_or_sp(self, index, value, wide=True):
        self.x[index] = value & (MASK64 if wide else MASK32)

    def set_flags_add(self, a, b, carry_in, width):
        mask = (1 << width) - 1
        unsigned = (a & mask) + (b & mask) + carry_in
        result = unsigned & mask
        signed = sign_extend(a & mask, width) + sign_extend(b & mask, width) + carry_in
        self.n = result >> (width - 1)
        self.z = int(result == 0)
        self.c = int(unsigned > mask)
        self.v_flag = int(sign_extend(result, width) != signed)
        return result

    def condition(self, code):
        base = code >> 1
        if base == 0:
            result = self.z == 1
        elif base == 1:
            result = self.c == 1
        elif base == 2:
            result = self.n == 1
        elif base == 3:
            result = self.v_flag == 1
        elif base == 4:
            result = self.c == 1 and self.z == 0
        elif base == 5:
            result = self.n == self.v_flag
        elif base == 6:
            result = self.n == self.v_flag and self.z == 0
        else:
            result = True
        if code & 1 and code != 15:
            result = not result
        return result

    def call(self, address, arguments=(), limit=50_000_000):
        for index, value in enumerate(arguments):
            self.x[index] = value & MASK64
        if self.x[31] == 0:
            self.x[31] = STACK_TOP
        self.x[30] = RETURN_SENTINEL
        try:
            self.run(address, limit)
        except StopRun:
            return None
        return self.x[0]

    def enter(self, target, return_address):
        name = self.call_names.get(target)
        if name is None and target not in self.call_names:
            name = self.library.name_of_call(target)
            self.call_names[target] = name
        if name is not None and name in self.stubs:
            self.x[0] = self.stubs[name](self) & MASK64
            return return_address
        if name is not None and name in self.library.address_of and self.may_run(name):
            target = self.library.address_of[name]
        elif name is not None:
            raise RunError("call to " + name + " with no stub")
        if target >= EXTERNAL_BASE:
            raise RunError("call into an external at " + hex(target))
        self.x[30] = return_address
        return target

    def run(self, pc, limit):
        memory = self.memory
        cache = {}
        while pc != RETURN_SENTINEL:
            self.executed += 1
            if self.executed > limit:
                raise RunError("ran past %d instructions at %s" % (limit, hex(pc)))
            word = cache.get(pc)
            if word is None:
                word = self.library.instruction(pc)
                if word is None:
                    raise RunError("no code at " + hex(pc))
                cache[pc] = word
            next_pc = pc + 4
            try:
                next_pc = self.step(pc, word, next_pc, memory)
            except RunError as error:
                raise RunError("%s at %s (%08x)" % (error, hex(pc), word)) from None
            pc = next_pc

    def step(self, pc, word, next_pc, memory):
        top = (word >> 25) & 0xf
        if top in (0x8, 0x9):
            return self.data_immediate(pc, word, next_pc)
        if top in (0xa, 0xb):
            return self.branch(pc, word, next_pc)
        if top in (0x4, 0x6, 0xc, 0xe):
            self.load_store(word, memory)
            return next_pc
        if top in (0x5, 0xd):
            self.data_register(word)
            return next_pc
        if top in (0x7, 0xf):
            self.float_op(word)
            return next_pc
        raise RunError("unknown instruction")

    def data_immediate(self, pc, word, next_pc):
        group = (word >> 23) & 0x7
        wide = bool(word >> 31)
        rd = word & 0x1f
        rn = (word >> 5) & 0x1f
        if group in (0, 1):
            if word >> 31:
                self.set_reg(rd, adrp_target(pc, word))
            else:
                immediate = ((word >> 29) & 3) | (((word >> 5) & 0x7ffff) << 2)
                self.set_reg(rd, pc + sign_extend(immediate, 21))
            return next_pc
        if group == 2:
            immediate = (word >> 10) & 0xfff
            if (word >> 22) & 1:
                immediate <<= 12
            subtract = (word >> 30) & 1
            set_flags = (word >> 29) & 1
            a = self.reg_or_sp(rn, wide)
            width = 64 if wide else 32
            if set_flags:
                result = self.set_flags_add(a, (~immediate if subtract else immediate), subtract, width)
                self.set_reg(rd, result, wide)
            else:
                result = (a - immediate) if subtract else (a + immediate)
                self.set_reg_or_sp(rd, result, wide)
            return next_pc
        if group == 4:
            width = 64 if wide else 32
            wmask, _ = decode_bit_masks((word >> 22) & 1, (word >> 10) & 0x3f, (word >> 16) & 0x3f, width)
            opc = (word >> 29) & 3
            a = self.reg(rn, wide)
            if opc == 0:
                result = a & wmask
            elif opc == 1:
                result = a | wmask
            elif opc == 2:
                result = a ^ wmask
            else:
                result = a & wmask
                self.n = result >> (width - 1)
                self.z = int(result == 0)
                self.c = self.v_flag = 0
                self.set_reg(rd, result, wide)
                return next_pc
            self.set_reg_or_sp(rd, result, wide)
            return next_pc
        if group == 5:
            opc = (word >> 29) & 3
            shift = ((word >> 21) & 3) * 16
            immediate = ((word >> 5) & 0xffff) << shift
            if opc == 0:
                self.set_reg(rd, ~immediate, wide)
            elif opc == 2:
                self.set_reg(rd, immediate, wide)
            elif opc == 3:
                current = self.reg(rd, wide)
                self.set_reg(rd, (current & ~(0xffff << shift)) | immediate, wide)
            else:
                raise RunError("unknown move wide")
            return next_pc
        if group == 6:
            width = 64 if wide else 32
            opc = (word >> 29) & 3
            immr = (word >> 16) & 0x3f
            imms = (word >> 10) & 0x3f
            wmask, tmask = decode_bit_masks((word >> 22) & 1, imms, immr, width)
            source = self.reg(rn, wide)
            rotated = ror(source, immr, width)
            if opc == 2:
                result = rotated & wmask & tmask
            elif opc == 0:
                bottom = rotated & wmask
                sign = (source >> imms) & 1
                top_bits = ((1 << width) - 1) if sign else 0
                result = (top_bits & ~tmask) | (bottom & tmask)
            else:
                destination = self.reg(rd, wide)
                bottom = (destination & ~wmask) | (rotated & wmask)
                result = (destination & ~tmask) | (bottom & tmask)
            self.set_reg(rd, result, wide)
            return next_pc
        raise RunError("unknown immediate data op")

    def branch(self, pc, word, next_pc):
        if (word & 0x7c000000) == 0x14000000:
            target = (pc + 4 * sign_extend(word & 0x3ffffff, 26)) & MASK64
            if word >> 31:
                return self.enter(target, next_pc)
            return target
        if (word & 0xff000010) == 0x54000000:
            if self.condition(word & 0xf):
                return (pc + 4 * sign_extend((word >> 5) & 0x7ffff, 19)) & MASK64
            return next_pc
        if (word & 0x7e000000) == 0x34000000:
            wide = bool(word >> 31)
            value = self.reg(word & 0x1f, wide)
            taken = (value != 0) if (word >> 24) & 1 else (value == 0)
            return (pc + 4 * sign_extend((word >> 5) & 0x7ffff, 19)) & MASK64 if taken else next_pc
        if (word & 0x7e000000) == 0x36000000:
            bit = ((word >> 31) << 5) | ((word >> 19) & 0x1f)
            value = (self.reg(word & 0x1f) >> bit) & 1
            taken = value == 1 if (word >> 24) & 1 else value == 0
            return (pc + 4 * sign_extend((word >> 5) & 0x3fff, 14)) & MASK64 if taken else next_pc
        if (word & 0xfffffc1f) == 0xd65f0000:
            return self.reg((word >> 5) & 0x1f)
        if (word & 0xfffffc1f) == 0xd61f0000:
            return self.reg((word >> 5) & 0x1f)
        if (word & 0xfffffc1f) == 0xd63f0000:
            if self.on_indirect_call is not None:
                self.on_indirect_call(self, self.reg((word >> 5) & 0x1f))
            return self.enter(self.reg((word >> 5) & 0x1f), next_pc)
        if (word & 0xfff00000) == 0xd5300000:
            self.set_reg(word & 0x1f, THREAD_BLOCK)
            return next_pc
        if (word & 0xfffff01f) == 0xd503201f:
            return next_pc
        raise RunError("unknown branch or system op")

    def shifted(self, value, kind, amount, width):
        mask = (1 << width) - 1
        if kind == 0:
            return (value << amount) & mask
        if kind == 1:
            return value >> amount
        if kind == 2:
            return (sign_extend(value, width) >> amount) & mask
        return ror(value, amount, width)

    def extended(self, value, option, amount):
        size = option & 3
        value &= (1 << (8 << size)) - 1 if size < 3 else MASK64
        if option & 4:
            value = sign_extend(value, 8 << size) & MASK64
        return (value << amount) & MASK64

    def data_register(self, word):
        wide = bool(word >> 31)
        width = 64 if wide else 32
        rd = word & 0x1f
        rn = (word >> 5) & 0x1f
        rm = (word >> 16) & 0x1f
        if (word & 0x1f000000) == 0x0a000000:
            amount = (word >> 10) & 0x3f
            operand = self.shifted(self.reg(rm, wide), (word >> 22) & 3, amount, width)
            if (word >> 21) & 1:
                operand = ~operand & ((1 << width) - 1)
            opc = (word >> 29) & 3
            a = self.reg(rn, wide)
            result = (a & operand, a | operand, a ^ operand, a & operand)[opc]
            if opc == 3:
                self.n = result >> (width - 1)
                self.z = int(result == 0)
                self.c = self.v_flag = 0
            self.set_reg(rd, result, wide)
            return
        if (word & 0x1f000000) == 0x0b000000:
            subtract = (word >> 30) & 1
            set_flags = (word >> 29) & 1
            if (word >> 21) & 1:
                operand = self.extended(self.reg(rm), (word >> 13) & 7, (word >> 10) & 7)
                a = self.reg_or_sp(rn, wide)
            else:
                operand = self.shifted(self.reg(rm, wide), (word >> 22) & 3, (word >> 10) & 0x3f, width)
                a = self.reg(rn, wide)
            if set_flags:
                result = self.set_flags_add(a, (~operand if subtract else operand), subtract, width)
                self.set_reg(rd, result, wide)
            else:
                result = (a - operand) if subtract else (a + operand)
                if (word >> 21) & 1:
                    self.set_reg_or_sp(rd, result, wide)
                else:
                    self.set_reg(rd, result, wide)
            return
        if (word & 0x1fe00000) == 0x1a800000:
            op2 = (word >> 10) & 3
            chosen = self.condition((word >> 12) & 0xf)
            if chosen:
                result = self.reg(rn, wide)
            else:
                result = self.reg(rm, wide)
                if (word >> 30) & 1:
                    result = ~result
                if op2 & 1:
                    result += 1
            self.set_reg(rd, result, wide)
            return
        if (word & 0x1fe00000) == 0x1ac00000:
            opcode = (word >> 10) & 0x3f
            a = self.reg(rn, wide)
            b = self.reg(rm, wide)
            if opcode in (2, 3):
                if b == 0:
                    result = 0
                elif opcode == 2:
                    result = a // b
                else:
                    sa, sb = sign_extend(a, width), sign_extend(b, width)
                    result = abs(sa) // abs(sb) * (1 if (sa < 0) == (sb < 0) else -1)
            elif opcode in (8, 9, 10, 11):
                result = self.shifted(a, opcode - 8, b % width, width)
            else:
                raise RunError("unknown two-source op")
            self.set_reg(rd, result, wide)
            return
        if (word & 0x1f000000) == 0x1b000000:
            ra = (word >> 10) & 0x1f
            product = self.reg(rn, wide) * self.reg(rm, wide)
            result = self.reg(ra, wide) - product if (word >> 15) & 1 else self.reg(ra, wide) + product
            self.set_reg(rd, result, wide)
            return
        raise RunError("unknown register data op")

    def load_store(self, word, memory):
        if (word & 0x3a000000) == 0x28000000:
            self.load_store_pair(word, memory)
            return
        if (word & 0x3f000000) == 0x08000000:
            size = 1 << (word >> 30)
            address = self.reg_or_sp((word >> 5) & 0x1f)
            if (word >> 22) & 1:
                self.set_reg(word & 0x1f, memory.u(address, size))
            else:
                memory.put(address, size, self.reg(word & 0x1f))
                if not (word >> 23) & 1:
                    self.set_reg((word >> 16) & 0x1f, 0, False)
            return
        if (word & 0x3b000000) == 0x18000000:
            raise RunError("literal loads are not supported")
        if (word & 0x3b000000) != 0x39000000 and (word & 0x3b200000) != 0x38000000 and (word & 0x3b200c00) != 0x38200800:
            raise RunError("unknown load or store")
        size = word >> 30
        vector = (word >> 26) & 1
        opc = (word >> 22) & 3
        rn = (word >> 5) & 0x1f
        rt = word & 0x1f
        base = self.reg_or_sp(rn)
        write_back = None
        if (word & 0x3b000000) == 0x39000000:
            scale = size + (4 if vector and opc >= 2 else 0)
            address = base + (((word >> 10) & 0xfff) << scale)
        elif (word >> 21) & 1:
            option = (word >> 13) & 7
            scale = size + (4 if vector and opc >= 2 else 0)
            address = base + self.extended(self.reg((word >> 16) & 0x1f), option, scale if (word >> 12) & 1 else 0)
        else:
            offset = sign_extend((word >> 12) & 0x1ff, 9)
            mode = (word >> 10) & 3
            if mode == 1:
                address = base
                write_back = base + offset
            elif mode == 3:
                address = base + offset
                write_back = address
            else:
                address = base + offset
        address &= MASK64
        if vector:
            bytes_count = (1 << size) if opc < 2 else 16
            if opc & 1:
                value = memory.u(address, bytes_count)
                self.v[rt] = value
            else:
                memory.put(address, bytes_count, self.v[rt])
        else:
            bytes_count = 1 << size
            if opc == 0:
                memory.put(address, bytes_count, self.reg(rt))
            elif opc == 1:
                self.set_reg(rt, memory.u(address, bytes_count))
            elif size == 3 and opc == 2:
                pass
            else:
                value = sign_extend(memory.u(address, bytes_count), 8 * bytes_count)
                self.set_reg(rt, value, opc == 2)
        if write_back is not None:
            self.set_reg_or_sp(rn, write_back)

    def load_store_pair(self, word, memory):
        opc = word >> 30
        vector = (word >> 26) & 1
        mode = (word >> 23) & 3
        load = (word >> 22) & 1
        if vector:
            scale = 2 + opc
        else:
            scale = 3 if opc == 2 else 2
        offset = sign_extend((word >> 15) & 0x7f, 7) << scale
        rt2 = (word >> 10) & 0x1f
        rn = (word >> 5) & 0x1f
        rt = word & 0x1f
        base = self.reg_or_sp(rn)
        address = base if mode == 1 else base + offset
        size = 1 << scale
        for index, register in enumerate((rt, rt2)):
            spot = (address + size * index) & MASK64
            if vector:
                if load:
                    self.v[register] = memory.u(spot, size)
                else:
                    memory.put(spot, size, self.v[register])
            elif load:
                value = memory.u(spot, size)
                if opc == 1:
                    value = sign_extend(value, 32) & MASK64
                self.set_reg(register, value)
            else:
                memory.put(spot, size, self.reg(register))
        if mode in (1, 3):
            self.set_reg_or_sp(rn, base + offset)

    def float_op(self, word):
        if (word & 0x9ff80400) == 0x0f000400:
            self.v[word & 0x1f] = simd_immediate(word)
            return
        if (word & 0xff20fc00) == 0x1e204000 or (word & 0xff20fc00) == 0x1e604000:
            self.v[word & 0x1f] = self.v[(word >> 5) & 0x1f] & (MASK32 if not (word >> 22) & 1 else MASK64)
            return
        if (word & 0xffe01fe0) == 0x1e201000:
            self.v[word & 0x1f] = float_immediate((word >> 13) & 0xff, False)
            return
        if (word & 0xffe01fe0) == 0x1e601000:
            self.v[word & 0x1f] = float_immediate((word >> 13) & 0xff, True)
            return
        if (word & 0xfffffc00) == 0x1e624000:
            value = struct.unpack("<d", struct.pack("<Q", self.v[(word >> 5) & 0x1f] & MASK64))[0]
            self.v[word & 0x1f] = struct.unpack("<I", struct.pack("<f", value))[0]
            return
        if (word & 0xfffffc00) == 0x1e22c000:
            value = struct.unpack("<f", struct.pack("<I", self.v[(word >> 5) & 0x1f] & MASK32))[0]
            self.v[word & 0x1f] = struct.unpack("<Q", struct.pack("<d", value))[0]
            return
        double = bool((word >> 22) & 1)
        rd = word & 0x1f
        rn = (word >> 5) & 0x1f
        if (word & 0xff200c00) in (0x1e200c00, 0x1e600c00):
            self.v[rd] = self.v[rn] if self.condition((word >> 12) & 0xf) else self.v[(word >> 16) & 0x1f]
            return
        if (word & 0xff207c00) in (0x1e204000, 0x1e604000) and ((word >> 15) & 0x3f) in (1, 2, 3, 8, 9, 10, 11, 12):
            value = unpack_float(self.v[rn], double)
            operation = (word >> 15) & 0x3f
            if operation == 1:
                value = abs(value)
            elif operation == 2:
                value = -value
            elif operation == 3:
                value = math.sqrt(value) if value >= 0 else float("nan")
            elif operation == 8:
                value = float(round(value))
            elif operation == 9:
                value = float(math.ceil(value))
            elif operation == 10:
                value = float(math.floor(value))
            elif operation == 11:
                value = float(math.trunc(value))
            else:
                value = float(math.floor(value + 0.5)) if value >= 0 else -float(math.floor(-value + 0.5))
            self.v[rd] = pack_float(value, double)
            return
        if (word & 0xff000000) == 0x1f000000:
            n_value = unpack_float(self.v[rn], double)
            m_value = unpack_float(self.v[(word >> 16) & 0x1f], double)
            a_value = unpack_float(self.v[(word >> 10) & 0x1f], double)
            product = n_value * m_value
            kind = (((word >> 21) & 1) << 1) | ((word >> 15) & 1)
            result = (a_value + product, a_value - product, -a_value - product, -a_value + product)[kind]
            self.v[rd] = pack_float(result, double)
            return
        if (word & 0xff20fc07) in (0x1e202000, 0x1e602000):
            a = unpack_float(self.v[rn], double)
            b = 0.0 if (word >> 3) & 1 else unpack_float(self.v[(word >> 16) & 0x1f], double)
            if a != a or b != b:
                self.n, self.z, self.c, self.v_flag = 0, 0, 1, 1
            elif a == b:
                self.n, self.z, self.c, self.v_flag = 0, 1, 1, 0
            elif a < b:
                self.n, self.z, self.c, self.v_flag = 1, 0, 0, 0
            else:
                self.n, self.z, self.c, self.v_flag = 0, 0, 1, 0
            return
        if (word & 0x7f3efc00) == 0x1e220000:
            wide = bool(word >> 31)
            value = self.reg(rn, wide)
            if not (word >> 16) & 1:
                value = sign_extend(value, 64 if wide else 32)
            self.v[rd] = pack_float(float(value), double)
            return
        if (word & 0x7f3efc00) == 0x1e380000:
            wide = bool(word >> 31)
            value = int(unpack_float(self.v[rn], double))
            self.set_reg(rd, value, wide)
            return
        if (word & 0xff200c00) in (0x1e200800, 0x1e600800):
            operation = (word >> 12) & 0xf
            a = unpack_float(self.v[rn], double)
            b = unpack_float(self.v[(word >> 16) & 0x1f], double)
            results = {0: lambda: a * b, 1: lambda: a / b if b else 0.0, 2: lambda: a + b, 3: lambda: a - b}
            if operation not in results:
                raise RunError("unknown two-source float op")
            self.v[rd] = pack_float(results[operation](), double)
            return
        if (word & 0x7f3ffc00) == 0x1e270000:
            self.v[word & 0x1f] = self.reg((word >> 5) & 0x1f, bool(word >> 31))
            return
        if (word & 0x7f3ffc00) == 0x1e260000:
            self.set_reg(word & 0x1f, self.v[(word >> 5) & 0x1f], bool(word >> 31))
            return
        raise RunError("unknown floating point op")

    def float_argument(self, index):
        return struct.unpack("<f", struct.pack("<I", self.v[index] & MASK32))[0]


def float_immediate(immediate, double):
    sign = immediate >> 7
    exponent = (immediate >> 4) & 7
    fraction = immediate & 0xf
    if exponent & 4:
        value = (16 + fraction) / 16.0 * (2.0 ** (exponent - 7))
    else:
        value = (16 + fraction) / 16.0 * (2.0 ** (exponent + 1))
    if sign:
        value = -value
    if double:
        return struct.unpack("<Q", struct.pack("<d", value))[0]
    return struct.unpack("<I", struct.pack("<f", value))[0]


def simd_immediate(word):
    full = bool((word >> 30) & 1)
    op = (word >> 29) & 1
    cmode = (word >> 12) & 0xf
    immediate = (((word >> 16) & 7) << 5) | ((word >> 5) & 0x1f)
    if cmode < 8 and cmode % 2 == 0:
        lane, width = immediate << (8 * (cmode >> 1)), 32
    elif cmode < 12 and cmode % 2 == 0:
        lane, width = immediate << (8 * ((cmode >> 1) & 1)), 16
    elif cmode == 12:
        lane, width = (immediate << 8) | 0xff, 32
    elif cmode == 13:
        lane, width = (immediate << 16) | 0xffff, 32
    elif cmode == 14 and op == 0:
        lane, width = immediate, 8
    elif cmode == 14:
        lane = 0
        for bit in range(8):
            if (immediate >> bit) & 1:
                lane |= 0xff << (8 * bit)
        lane, width = lane, 64
    elif cmode == 15 and op == 0:
        lane, width = float_immediate(immediate, False), 32
    elif cmode == 15:
        lane, width = float_immediate(immediate, True), 64
    else:
        raise RunError("unsupported vector immediate")
    if op and cmode < 12:
        lane = ~lane & ((1 << width) - 1)
    value = 0
    for _ in range(64 // width):
        value = (value << width) | lane
    return value | (value << 64) if full else value


def pack_float(value, double):
    if double:
        return struct.unpack("<Q", struct.pack("<d", value))[0]
    return struct.unpack("<I", struct.pack("<f", value))[0]


def unpack_float(raw, double):
    if double:
        return struct.unpack("<d", struct.pack("<Q", raw & MASK64))[0]
    return struct.unpack("<f", struct.pack("<I", raw & MASK32))[0]

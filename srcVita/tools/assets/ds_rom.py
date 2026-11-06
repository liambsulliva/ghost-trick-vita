import os
import struct

GAME_CODE_OFFSET = 12
GAME_CODE_SIZE = 4
ACCEPTED_GAME_CODE = b"BGTE"
TABLES_OFFSET = 0x40
HEADER_SIZE = 0x50
ALLOCATION_ENTRY_SIZE = 8
DIRECTORY_ENTRY_SIZE = 8
ROOT_DIRECTORY_INDEX = 0
FIRST_DIRECTORY_ID = 0xF000
SUBDIRECTORY_BIT = 0x80
NAME_LENGTH_MASK = 0x7F


class RomRefused(ValueError):
    pass


class DsRom:
    def __init__(self, path):
        self.source = open(path, "rb")
        try:
            self.size = os.fstat(self.source.fileno()).st_size
            self.read_tables()
        except BaseException:
            self.source.close()
            raise

    def __enter__(self):
        return self

    def __exit__(self, *_details):
        self.source.close()

    def read_range(self, offset, size):
        if size < 0 or offset + size > self.size:
            raise RomRefused("the DS ROM's file table points outside the file")
        self.source.seek(offset)
        return self.source.read(size)

    def read_tables(self):
        if self.size < HEADER_SIZE:
            raise RomRefused("this file is too short to be a DS ROM")
        header = self.read_range(0, HEADER_SIZE)
        if header[GAME_CODE_OFFSET:GAME_CODE_OFFSET + GAME_CODE_SIZE] != ACCEPTED_GAME_CODE:
            raise RomRefused("this DS ROM is not the USA cartridge. Only USA cartridges are accepted for now, because no other region's scripts have been checked, so nothing was converted. Remove this file or name another one with --ds-rom")
        names_offset, names_size, self.allocation_offset, self.allocation_size = struct.unpack_from("<IIII", header, TABLES_OFFSET)
        self.names = self.read_range(names_offset, names_size)

    def directory_entries(self, directory_index):
        try:
            position, file_id = struct.unpack_from("<IH", self.names, directory_index * DIRECTORY_ENTRY_SIZE)
            while self.names[position] != 0:
                kind = self.names[position]
                name_end = position + 1 + (kind & NAME_LENGTH_MASK)
                name = self.names[position + 1:name_end].decode("latin-1")
                position = name_end
                if kind & SUBDIRECTORY_BIT:
                    child_id = struct.unpack_from("<H", self.names, position)[0]
                    position = position + 2
                    yield name, None, child_id - FIRST_DIRECTORY_ID
                else:
                    yield name, file_id, None
                    file_id = file_id + 1
        except (IndexError, struct.error):
            raise RomRefused("the DS ROM's file name table is damaged") from None

    def file_id_of(self, path):
        parts = path.split("/")
        directory_index = ROOT_DIRECTORY_INDEX
        for part in parts[:-1]:
            directory_index = next((child for name, _, child in self.directory_entries(directory_index) if name == part and child is not None), None)
            if directory_index is None:
                return None
        return next((file_id for name, file_id, _ in self.directory_entries(directory_index) if name == parts[-1] and file_id is not None), None)

    def read_file(self, path):
        file_id = self.file_id_of(path)
        if file_id is None:
            raise RomRefused("the DS ROM has no file named " + path)
        if (file_id + 1) * ALLOCATION_ENTRY_SIZE > self.allocation_size:
            raise RomRefused("the DS ROM's file table does not cover " + path)
        start, end = struct.unpack("<II", self.read_range(self.allocation_offset + file_id * ALLOCATION_ENTRY_SIZE, ALLOCATION_ENTRY_SIZE))
        return self.read_range(start, end - start)

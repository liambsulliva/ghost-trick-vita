typedef struct {
    void *archive;
    unsigned int file;
} FileId;

struct Key {
    unsigned int idx : 24;
    unsigned int hi : 8;
};

struct Header {
    char pad0[0x18];
    int data;
    int f1c;
    int table;
    int f24;
    int end;
    int f2c;
};

struct Sections {
    char pad0[8];
    int third;
    int fc;
    int first;
    int second;
    char pad18[0x10];
};

struct Ref {
    int index;
    int f4;
    int f8;
};

struct Entry {
    int offset;
    unsigned int size;
    int f8;
    int fc;
};

struct Block {
    char pad0[0x38];
    int count;
};

extern char data_020b18a0;
extern int func_02000cc4(const char *path, void *file, FileId *id);
extern int func_0206c614(void *file);
extern int func_02000ce8(void *file, void *buffer, int size, int position);
extern int func_020018b0(void);

#define WORK() (*(char **)((char *)&data_020b18a0 + 0x10))

int func_02001600(const char *path)
{
    char file[0x48];
    struct Header header;
    struct Sections sections;
    struct Entry entry;
    struct Ref ref;
    struct Block block;
    struct Ref ref2;
    FileId id;
    unsigned int count;
    int offset;
    struct Key key;
    unsigned int i;
    int total;

    if (func_02000cc4(path, file, &id) == 0) {
        return -3;
    }
    if (func_02000ce8(file, &header, 0x30, 0) < 0x30u) {
        func_0206c614(file);
        return -3;
    }
    total = header.end - header.data + 0xa0;
    *(int *)(WORK() + 0x15a8) = total;
    total = (total + 0x1f) & ~0x1f;
    if (func_02000ce8(file, &sections, 0x28, header.data) < 0x28u) {
        func_0206c614(file);
        return -2;
    }
    if (func_02000ce8(file, &count, 4, header.data + sections.first) < 4u) {
        func_0206c614(file);
        return -2;
    }
    for (i = 0; i < count; i++) {
        if (func_02000ce8(file, &offset, 4, header.data + sections.first + 4 + i * 4) < 4u) {
            func_0206c614(file);
            return -2;
        }
        if (offset != 0) {
            if (func_02000ce8(file, &ref, 0xc, header.data + offset) < 0xcu) {
                func_0206c614(file);
                return -2;
            }
            if (func_02000ce8(file, &entry, 0x10, header.table + 0xc + ref.index * 16) < 0x10u) {
                func_0206c614(file);
                return -2;
            }
            if (*(unsigned int *)(WORK() + 0x15b0) < entry.size) {
                *(unsigned int *)(WORK() + 0x15b0) = entry.size;
            }
        }
    }
    if (func_02000ce8(file, &count, 4, header.data + sections.second) < 4u) {
        func_0206c614(file);
        return -2;
    }
    for (i = 0; i < count; i++) {
        if (func_02000ce8(file, &offset, 4, header.data + sections.second + 4 + i * 4) < 4u) {
            func_0206c614(file);
            return -2;
        }
        if (offset != 0) {
            if (func_02000ce8(file, &key, 4, header.data + offset) < 4u) {
                func_0206c614(file);
                return -2;
            }
            if (key.hi & 1) {
                *(int *)(WORK() + 0x1b8c) += 1;
                if (func_02000ce8(file, &entry, 0x10, header.table + 0xc + key.idx * 16) < 0x10u) {
                    func_0206c614(file);
                    return -2;
                }
                if (func_02000ce8(file, &block, 0x3c, entry.offset) < 0x3cu) {
                    func_0206c614(file);
                    return -2;
                }
                *(int *)(WORK() + 0x1b88) = *(int *)(WORK() + 0x1b88) + (((unsigned int)((block.count * 2 + 0x20) * 4 + 0x3c) + 0x1f) & ~0x1f);
            }
        }
    }
    if (func_02000ce8(file, &count, 4, header.data + sections.third) < 4u) {
        func_0206c614(file);
        return -2;
    }
    for (i = 0; i < count; i++) {
        if (func_02000ce8(file, &offset, 4, header.data + sections.third + 4 + i * 4) < 4u) {
            func_0206c614(file);
            return -2;
        }
        if (offset != 0) {
            if (func_02000ce8(file, &ref2, 0xc, header.data + offset) < 0xcu) {
                func_0206c614(file);
                return -2;
            }
            *(int *)(WORK() + 0x1b94) += 1;
            if (func_02000ce8(file, &entry, 0x10, header.table + 0xc + ref2.index * 16) < 0x10u) {
                func_0206c614(file);
                return -2;
            }
            if (*(unsigned int *)(WORK() + 0x15ac) < entry.size) {
                *(unsigned int *)(WORK() + 0x15ac) = entry.size;
            }
        }
    }
    *(int *)(WORK() + 0x1b90) = ((count + 1 + *(int *)(WORK() + 0x1b94) * 32) * 4 + 0x1f) & ~0x1f;
    if (func_0206c614(file) == 0) {
        total = -3;
    }
    if (total > 0) {
        return func_020018b0();
    }
    return total;
}

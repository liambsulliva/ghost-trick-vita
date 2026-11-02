struct Key {
    unsigned int idx : 24;
    unsigned int hi : 8;
};

struct Entry {
    unsigned int idx;
    int size;
};

extern char data_020b18a0;
extern int func_02000cc4(const char *path, void *file, void *id);
extern void func_0206c614(void *file);
extern int func_02000ce8(void *file, void *buffer, int size, int position);
extern struct Key *func_02000d9c(char *pack, unsigned int index, unsigned int table);
extern int func_02000e00(char *pack, struct Key *key);

#define WORK() (*(char **)((char *)&data_020b18a0 + 0x10))
#define PACK() (*(char **)(WORK() + 0x1580))

int func_02001470(const char *path, char *pack)
{
    int result;
    char *stream;
    int size;
    int read;
    char *header;
    int offset;
    unsigned int i;
    char *cursor;
    struct Key *key;

    PACK() = pack;
    stream = PACK() + 0x34;
    if (func_02000cc4(path, stream, PACK() + 0x7c) == 0) {
        func_0206c614(stream);
        return -3;
    }
    result = 0;
    if (func_02000ce8(stream, pack, 0x30, 0) < 0x30u) {
        return result - 2;
    }
    size = *(int *)(pack + 0x28) - *(int *)(pack + 0x18);
    *(int *)(WORK() + 0x15a8) = size + 0xa0;
    read = func_02000ce8(stream, pack + 0xa0, size, *(int *)(pack + 0x18));
    if (read < size) {
        return result - 2;
    }
    *(int *)(PACK() + 0x94) = 0;
    *(char **)(PACK() + 0x98) = PACK() + 0xa0;
    *(char **)(PACK() + 0x90) = *(char **)(PACK() + 0x98) + *(int *)(PACK() + 0x1c);
    header = *(char **)(PACK() + 0x98);
    offset = *(int *)(header + 0x14);
    cursor = (char *)((int)(pack + *(int *)(WORK() + 0x15a8) + 0x1f) & ~0x1f);
    *(char **)(WORK() + 0x15b4) = cursor;
    cursor = (char *)((int)(cursor + *(int *)(WORK() + 0x15b0) + 0x1f) & ~0x1f);
    *(char **)(WORK() + 0x1b84) = cursor;
    cursor = (char *)((int)(cursor + *(int *)(WORK() + 0x1b8c) * 8 + 0x1f) & ~0x1f);
    for (i = 0; i < *(unsigned int *)(header + offset); i++) {
        key = func_02000d9c(PACK(), i, *(unsigned int *)(*(char **)(PACK() + 0x98) + 0x14));
        if (key->hi & 1) {
            *(char **)(*(char **)(PACK() + 0x90) + key->idx * 0x10 + 0x14) = cursor;
            ((struct Entry *)*(char **)(WORK() + 0x1b84))[i].idx = key->idx;
            ((struct Entry *)*(char **)(WORK() + 0x1b84))[i].size = *(int *)(*(char **)(PACK() + 0x90) + key->idx * 0x10 + 0x10);
            *(int *)(*(char **)(PACK() + 0x90) + key->idx * 0x10 + 0x10) = -1;
            cursor = cursor + ((func_02000e00(PACK(), key) + 0x1f) & ~0x1f);
        }
    }
    *(char **)(WORK() + 0x15b8) = cursor;
    return read;
}

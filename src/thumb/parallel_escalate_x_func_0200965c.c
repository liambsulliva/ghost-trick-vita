struct SaveHeader {
    char magic[0x20];
    unsigned char language;
    char pad21[3];
};

struct SaveSlot {
    int offset;
    int stride;
};

struct SaveFile {
    char pad0[9];
    unsigned char loaded;
    char pad0a[0x10];
    unsigned char reset;
    char pad1b[0x14e0 - 0x1b];
    char data[1][1][0x20];
};

extern int func_02009040(struct SaveFile *file, void *dest, int size, int offset);
extern int func_020090a4(struct SaveFile *file, void *src, int size, int offset);
extern void func_02008ea4(void *dest, int size);
extern int func_02079180(const char *a, const char *b, int n);
extern char *func_02078e84(char *dest, const char *src);
extern char data_02083a98[];
extern char data_02083ab8[];
extern const struct SaveSlot data_02083a8c[];
extern unsigned char data_020cff60;

void func_0200965c(struct SaveFile *file, int create)
{
    struct SaveHeader header;
    char key[0x10];
    unsigned int i;
    unsigned int j;

    if (file->loaded == 1) {
        return;
    }
    if (func_02009040(file, &header, sizeof(header), 0) != 1) {
        return;
    }
    if (func_02079180(header.magic, data_02083a98, 0x1e) != 0) {
        if (func_02079180(header.magic, data_02083ab8, 0x14) == 0 && create == 0) {
            file->reset = 1;
        }
        if (create == 0) {
            return;
        }
        func_02008ea4(key, sizeof(key));
        func_020090a4(file, key, sizeof(key), 0x60);
        func_02078e84(header.magic, data_02083a98);
        header.language = data_020cff60;
        func_020090a4(file, &header, sizeof(header), 0);
        return;
    }
    if (header.language >= 1 && header.language <= 5) {
        data_020cff60 = header.language;
    }
    file->loaded = 1;
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 1; j++) {
            if (func_02009040(file, file->data[i][j], 0x20, data_02083a8c[i].offset + j * data_02083a8c[i].stride) != 1) {
                return;
            }
        }
    }
}

typedef struct Item02108 {
    unsigned short id;
    char pad2[0xa];
} Item02108;

typedef struct Header02108 {
    char pad0[0x1c];
    unsigned int count;
    char pad20[4];
    Item02108 items[1];
} Header02108;

typedef struct File02108 {
    unsigned int size;
    Header02108 *data;
    int pad8[2];
} File02108;

typedef struct Table02108 {
    char pad0[0x10];
    File02108 f[1];
} Table02108;

typedef struct Info02108 {
    char pad0[0xc];
    int key;
} Info02108;

typedef struct Archive02108 {
    char pad0[0x90];
    Table02108 *files;
    int pad94;
    Info02108 *info;
} Archive02108;

typedef struct Bank02108 {
    char pad0[0x1580];
    Archive02108 *archive;
    char pad1584[0x20];
    unsigned int flags;
} Bank02108;

typedef struct Game02108 {
    char pad0[0x10];
    Bank02108 *bank;
} Game02108;

extern Game02108 data_020b18a0;
extern int *func_02000d9c(Archive02108 *arc, int name, int key);
extern unsigned int func_02000d64(Archive02108 *arc, int file, int dst, unsigned int size, int zero);
extern int func_020021e8(unsigned short id, int dst, int flags);

int func_02002108(int name, int dst, int flags)
{
    int start;
    int *file;
    unsigned int n;
    Header02108 *hdr;
    unsigned short i;
    int r;

    start = dst;
    file = func_02000d9c(data_020b18a0.bank->archive, name, data_020b18a0.bank->archive->info->key);
    if (file == 0) {
        return -5;
    }
    if (data_020b18a0.bank->archive->files->f[*file].data == 0) {
        n = func_02000d64(data_020b18a0.bank->archive, *file, dst, data_020b18a0.bank->archive->files->f[*file].size, 0);
        if (n < data_020b18a0.bank->archive->files->f[*file].size) {
            return -2;
        }
        data_020b18a0.bank->archive->files->f[*file].data = (Header02108 *)dst;
        dst += n;
    }
    hdr = data_020b18a0.bank->archive->files->f[*file].data;
    if (data_020b18a0.bank->flags & 0x80000000) {
        data_020b18a0.bank->flags |= 0x40000000;
    }
    if (flags & 2) {
        for (i = 0; i < hdr->count; i++) {
            r = func_020021e8(hdr->items[i].id, dst, flags);
            if (r == -3 || r == -2) {
                return r;
            }
            dst += r;
        }
    }
    return dst - start;
}

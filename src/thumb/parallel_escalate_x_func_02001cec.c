typedef struct File01cec {
    unsigned int size;
    void *data;
    int pad8[2];
} File01cec;

typedef struct Table01cec {
    char pad0[0x10];
    File01cec f[1];
} Table01cec;

typedef struct Info01cec {
    char pad0[8];
    int list;
    int pc;
    int key_a;
    int key_b;
} Info01cec;

typedef struct Archive01cec {
    char pad0[0x90];
    Table01cec *files;
    int pad94;
    Info01cec *info;
} Archive01cec;

typedef struct Bank01cec {
    char pad0[0x13e0];
    char mask[0x20];
    char pad1400[0x140];
    char slots[0x40];
    Archive01cec *archive;
    char pad1584[0x20];
    unsigned int flags;
    char pad15a8[0xc];
    void *dst;
    int *base;
    char pad15bc[0x5d4];
    int size;
    char pad1b94[0x10];
    int count;
    int ids;
} Bank01cec;

typedef struct Game01cec {
    char pad0[0x10];
    Bank01cec *bank;
} Game01cec;

struct List {
    unsigned int count;
    int offsets[1];
};

struct Rec {
    int file;
    unsigned short id;
};

struct Res {
    int file;
    unsigned short ids[4];
};

struct Key {
    unsigned int idx : 24;
    unsigned int hi : 8;
};

struct Save {
    char mask[0x20];
    int count;
    int ids;
    char slots[0x40];
};

extern Game01cec data_020b18a0;
extern void *func_02000d9c(Archive01cec *arc, int name, int key);
extern unsigned int func_02000d64(Archive01cec *arc, int file, void *dst, unsigned int size, int zero);
extern void func_02001fa0(void *object, int value);
extern void func_02001fac(void *object, int value, int size);
extern int func_02003444(unsigned int index);
extern int func_020034cc(unsigned int index, int id, int slot);
extern void func_02000f70(void *dest, int value, int size);
extern void func_02001468(void *src, void *dst, int size);
extern void func_02003560(void *dst, unsigned int index);

int func_02001cec(void *dst)
{
    struct Save *cursor;
    Info01cec *info;
    unsigned int i;
    struct List *list;
    int prev;
    struct Rec *rec;
    struct Res *res;
    unsigned int n2;
    int *base;
    unsigned int n;
    unsigned int j;
    unsigned short k;

    prev = 0xffff;
    info = data_020b18a0.bank->archive->info;
    list = (struct List *)((char *)info + info->list);
    base = data_020b18a0.bank->base;
    cursor = (struct Save *)(((unsigned int)((char *)(base + list->count) + 4) + 0x1f) & ~0x1f);
    func_02001fa0(base, data_020b18a0.bank->size);
    *base = list->count;
    for (i = 0; i < list->count; i++) {
        if (list->offsets[i] == 0) {
            continue;
        }
        base[i + 1] = (char *)cursor - (char *)base;
        rec = (struct Rec *)((char *)info + list->offsets[i]);
        n = func_02000d64(data_020b18a0.bank->archive, rec->file, dst, data_020b18a0.bank->archive->files->f[rec->file].size, 0);
        if (n < data_020b18a0.bank->archive->files->f[rec->file].size) {
            return -2;
        }
        data_020b18a0.bank->archive->files->f[rec->file].data = dst;
        func_02001fac(data_020b18a0.bank->mask, 0, 0x20);
        if (func_02003444(i) == 0) {
            func_02001fac(data_020b18a0.bank->mask, -1, 0x20);
        }
        data_020b18a0.bank->count = 0;
        data_020b18a0.bank->ids = 0;
        func_02001fac(data_020b18a0.bank->slots, 0, 0x40);
        res = func_02000d9c(data_020b18a0.bank->archive, rec->id, data_020b18a0.bank->archive->info->key_a);
        if (prev != rec->id) {
            n = func_02000d64(data_020b18a0.bank->archive, res->file, data_020b18a0.bank->dst, data_020b18a0.bank->archive->files->f[res->file].size, 0);
            if (n < data_020b18a0.bank->archive->files->f[res->file].size) {
                return -2;
            }
        }
        prev = rec->id;
        for (j = 0; j < 4; j++) {
            if (res->ids[j] != 0xffff) {
                if (((struct Key *)func_02000d9c(data_020b18a0.bank->archive, res->ids[j], data_020b18a0.bank->archive->info->key_b))->hi & 1) {
                    func_02003560(data_020b18a0.bank->dst, j);
                }
            }
        }
        n2 = (unsigned short)data_020b18a0.bank->count;
        if (n2 > 4) {
            n2 = 4;
        }
        for (k = 0; k < n2; k++) {
            if (func_020034cc(i, ((unsigned char *)&data_020b18a0.bank->ids)[k], k) == 0) {
                func_02000f70(data_020b18a0.bank->slots, -1, 0x40);
                break;
            }
        }
        func_02001468(data_020b18a0.bank->mask, cursor, 0x20);
        cursor->count = data_020b18a0.bank->count;
        cursor->ids = data_020b18a0.bank->ids;
        func_02001468(data_020b18a0.bank->slots, cursor->slots, 0x40);
        data_020b18a0.bank->archive->files->f[rec->file].data = 0;
        cursor++;
    }
    data_020b18a0.bank->flags |= 0x8000;
    return 0;
}

#pragma opt_common_subs off
#pragma opt_lifetimes off
#pragma opt_loop_invariants off
typedef struct File021e8 {
    unsigned int size;
    void *data;
    int pad8[2];
} File021e8;

typedef struct Table021e8 {
    char pad0[8];
    unsigned int count;
    int padc;
    File021e8 f[1];
} Table021e8;

typedef struct Info021e8 {
    char pad0[0x10];
    int key_a;
    int key_b;
} Info021e8;

typedef struct Archive021e8 {
    char pad0[0x90];
    Table021e8 *files;
    int pad94;
    Info021e8 *info;
} Archive021e8;

typedef struct Bank021e8 {
    char pad0[0x13e0];
    unsigned int mask[8];
    char pad1400[0x100];
    unsigned int bits[0x10];
    unsigned int slots[4][4];
    Archive021e8 *archive;
    char pad1584[0x20];
    unsigned int flags;
    char pad15a8[0x10];
    int *base;
    char pad15bc[0x5e8];
    int count;
    int ids;
} Bank021e8;

typedef struct Game021e8 {
    char pad0[0x10];
    Bank021e8 *bank;
} Game021e8;

struct Res {
    unsigned int file;
    unsigned short ids[4];
};

struct Key {
    unsigned int idx : 24;
    unsigned int hi : 8;
};

struct Save {
    unsigned int mask[8];
    int count;
    int ids;
    unsigned int slots[4][4];
};

extern Game021e8 data_020b18a0;
extern void *func_02000d9c(Archive021e8 *arc, int name, int key);
extern unsigned int func_02000d64(Archive021e8 *arc, int file, int dst, unsigned int size, int zero);
extern int func_02002564(int id, int dst);
extern void func_02007a0c(void *hdr, unsigned int index, void *partner);
extern void func_02000f70(void *dest, int value, int size);
extern void func_02001fac(void *object, int value, int size);
extern int func_02003444(unsigned int index);
extern int func_020034cc(unsigned int index, int id, int slot);
extern void func_02003560(void *dst, unsigned int index);

int func_020021e8(int id, int dst, unsigned char flags)
{
    void *hdr;
    int n;
    void *partner;
    struct Res *file;
    Game021e8 *game;
    unsigned short k;
    unsigned short i;
    int start;
    int res;
    struct Save *save;
    unsigned short j;
    unsigned short n2;
    unsigned short i1;
    struct Key *key;

    int bit;
    Table021e8 * files;
    int idx;



    unsigned short k2;


    game = &data_020b18a0;
    start = dst;
    n = 0;
    file = func_02000d9c(game->bank->archive, id, game->bank->archive->info->key_a);
    idx = file->file;
    files = game->bank->archive->files;
    if (idx >= files->count) {
        return -10;
    }
    if (files->f[idx].data == 0) {
        n = func_02000d64(game->bank->archive, file->file, dst, game->bank->archive->files->f[file->file].size, 0);
        if (n < game->bank->archive->files->f[file->file].size) {
            return -2;
        }
        game->bank->archive->files->f[file->file].data = (void *)dst;
    }
    hdr = data_020b18a0.bank->archive->files->f[file->file].data;
    if (flags & 4) {
        dst += (n + 0x1f) & ~0x1f;
        dst += func_02002564(id, dst);
    } else {
        dst += n;
        for (i = 0; i < 4; i = i + 1) {
            if (file->ids[i] != 0xffff) {
                key = func_02000d9c(data_020b18a0.bank->archive, file->ids[i], data_020b18a0.bank->archive->info->key_b);
                if ((partner = data_020b18a0.bank->archive->files->f[key->idx].data) != 0 && data_020b18a0.bank->archive->files->f[key->idx].size != -1) {
                    func_02007a0c(hdr, i, partner);
                }
            }
        }
        if ((data_020b18a0.bank->flags & 0x8000) == 0) {
            func_02000f70(data_020b18a0.bank->mask, 0, 0x20);
            for (i1 = 0; i1 < 0x200; i1 = i1 + 1) {
                bit = i1;
                if ((data_020b18a0.bank->bits[bit / 32] >> (bit - bit / 32 * 32)) & 1) {
                    if (func_02003444(i1) == 0) {
                        func_02000f70(data_020b18a0.bank->mask, -1, 0x20);
                        break;
                    }
                }
            }
            data_020b18a0.bank->count = 0;
            data_020b18a0.bank->ids = 0;
            func_02000f70(data_020b18a0.bank->slots, 0, 0x40);
            for (i = 0; i < 4; i = i + 1) {
                if (file->ids[i] != 0xffff) {
                    if (((struct Key *)func_02000d9c(data_020b18a0.bank->archive, file->ids[i], data_020b18a0.bank->archive->info->key_b))->hi & 1) {
                        func_02003560(hdr, i);
                    }
                }
            }
            for (i = 0; i < 0x200; i = i + 1) {
                bit = i;
                if ((data_020b18a0.bank->bits[bit / 32] >> (bit - bit / 32 * 32)) & 1) {
                    n2 = data_020b18a0.bank->count;
                    if (n2 > 4) {
                        n2 = 4;
                    }
                    for (k = 0; k < n2; k = k + 1) {
                        res = func_020034cc(i, ((unsigned char *)&data_020b18a0.bank->ids)[k], k);
                        if (res == 0) {
                            func_02000f70(data_020b18a0.bank->slots, -1, 0x40);
                            break;
                        }
                    }
                    if (res == 0) {
                        break;
                    }
                }
            }
        } else {
            func_02001fac(data_020b18a0.bank->mask, 0, 0x20);
            func_02001fac(data_020b18a0.bank->slots, 0, 0x40);
            for (i = 0; i < 0x200; i = i + 1) {
                bit = i;
                if ((data_020b18a0.bank->bits[bit / 32] >> (bit - bit / 32 * 32)) & 1) {
                    save = (struct Save *)((char *)data_020b18a0.bank->base + (data_020b18a0.bank->base + i)[1]);
                    for (j = 0; j < 8; j = j + 1) {
                        ((unsigned int *)data_020b18a0.bank->mask)[j] |= save->mask[j];
                    }
                    data_020b18a0.bank->count = save->count;
                    data_020b18a0.bank->ids = save->ids;
                    for (j = 0; j < 4; j = j + 1) {
                        for (k2 = 0; k2 < 4; k2 = k2 + 1) {
                            ((unsigned int *)data_020b18a0.bank->slots[j])[k2] |= save->slots[j][k2];
                        }
                    }
                }
            }
        }
    }
    return dst - start;
}

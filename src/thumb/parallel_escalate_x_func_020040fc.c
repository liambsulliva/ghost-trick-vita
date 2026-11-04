typedef struct Slot040fc {
    char pad0[4];
    int mode;
    char pad8[6];
    unsigned char volume;
    char padf[3];
    unsigned char group;
    char pad13[0x1e];
    unsigned char level;
    char pad32[0x1a];
} Slot040fc;

typedef struct Bank040fc {
    char pad[0x15bc];
    Slot040fc slots[16];
} Bank040fc;

typedef struct Game040fc {
    char pad[0x10];
    Bank040fc *bank;
} Game040fc;

extern Game040fc data_020b18a0;
extern unsigned int func_02002ff4(unsigned int a);
extern unsigned int func_02003030(unsigned int a);
extern unsigned int func_0200306c(unsigned int a);
extern unsigned int func_0207b424(unsigned int a);
extern unsigned int func_0207b3dc(int a);
extern unsigned int func_0207bb44(unsigned int a, unsigned int b);
extern unsigned int func_0207c1d8(unsigned int a, unsigned int b);
extern int func_0207ba84(unsigned int a);
extern void func_02003624(int slot, int flags, unsigned char a, unsigned char b, unsigned char level);
extern void func_020036e4(int slot, unsigned char a, int flags, short b, signed char c, short d, int e, int f);

void func_020040fc(unsigned int p0, unsigned int p1, unsigned int p2, unsigned char p3, int flags, unsigned char p6, unsigned char p7, int vol, short p9, signed char p10, short p11, unsigned char group, int mode)
{
    unsigned int mask;
    short i;
    Slot040fc *s;
    int v;

    mask = 0xffff;
    if (p0 != 0xffffffff) {
        mask &= func_02002ff4(p0);
    }
    if (p1 != 0xffffffff) {
        mask &= func_02003030(p1);
    }
    if (p2 != 0xffffffff) {
        mask &= func_0200306c(p2);
    }
    for (i = 0; i < 16; i++) {
        s = &data_020b18a0.bank->slots[i];
        if ((((int)mask >> i) & 1) == 0) {
            continue;
        }
        if (group != s->group && group != 0xff) {
            continue;
        }
        switch (mode) {
        case 0:
        case 1:
            if (s->mode != mode) {
                continue;
            }
        }
        if (flags & 2) {
            v = s->volume + vol;
        } else if (flags & 4) {
            v = func_0207ba84(func_0207c1d8(func_0207bb44(func_0207b424(s->volume), func_0207b3dc(vol)), 0x42fe0000));
        } else {
            v = vol;
        }
        if (v < 0) {
            v = 0;
        } else if (v > 0x7f) {
            v = 0x7f;
        }
        s->level = v;
        func_02003624(i, flags, p6, p7, s->level);
        func_020036e4(i, p3, flags, p9, p10, p11, 0, 0);
    }
}

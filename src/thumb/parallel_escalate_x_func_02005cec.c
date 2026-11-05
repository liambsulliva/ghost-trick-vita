struct State {
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern int *func_02000d9c(int obj, int key, int c);
extern void func_02005654(int id);
extern int func_0207b3dc(unsigned int value);
extern int func_0207b424(int value);
extern int func_0207c1d8(int a, int b);

int func_02005cec(int key, int owner, int target, int volume, int mode, int pan)
{
    unsigned char *base;
    int *src;
    unsigned char *fade;
    unsigned int flags;
    int obj;
    int frames;

    frames = (int)(long long)target;
    obj = *(int *)(data_020b18a0.data + 0x1580);
    src = func_02000d9c(obj, key, *(int *)(*(int *)(obj + 0x98) + 0x24));
    if (src == 0) {
        return -5;
    }
    base = (unsigned char *)(data_020b18a0.data + 0x3d28 + ((unsigned char *)src)[6] * 0x3a4);
    flags = *(unsigned int *)(base + 0xe8);
    if ((flags & 1) == 0) {
        return -6;
    }
    if (base[0x61] > ((unsigned char *)src)[5]) {
        return -4;
    }
    switch (mode) {
    case 0:
        if (volume == -1 || volume > *(unsigned short *)(base + 0xfc)) {
            volume = *(unsigned short *)(base + 0xfc);
        }
        *(int *)(base + 0x110) = volume;
        *(unsigned int *)(base + 0xe8) &= ~8;
        *(int *)(base + 0x100) = 0;
        *(int *)(base + 0x104) = 0;
        *(unsigned int *)(data_020b18a0.data + 0x4bc0 + ((unsigned char *)src)[6] * 0x24) &= ~2;
        break;
    case 1:
        if (volume == -1 || volume > *(unsigned short *)(base + 0xfc)) {
            volume = *(unsigned short *)(base + 0xfc);
        }
        *(int *)(base + 0x110) = volume;
        *(unsigned int *)(base + 0xe8) |= 8;
        break;
    case 2:
        if (flags & 8) {
            *(unsigned int *)(base + 0xe8) &= ~8;
        }
        *(unsigned int *)(base + 0xe8) &= ~4;
        *(int *)(base + 0x84) = 3;
        if (frames != 0) {
            *(int *)(base + 0x70) = frames;
            fade = base + 0x70;
            fade[0xc] = base[0xf2];
            *(int *)(fade + 4) = 0;
            *(int *)(fade + 8) = func_0207c1d8(func_0207b3dc(fade[0xc]), func_0207b424(*(int *)(base + 0x70)));
            *(int *)(fade + 0x10) = 1;
            *(int *)(fade + 0x14) = 3;
            fade[0xd] = 8;
            *(unsigned int *)(base + 0xe8) |= 4;
            base[0x60] = 0;
        }
        return 0;
    }
    switch (*(int *)(base + 0xec)) {
    case 5:
    case 6:
    case 10:
        func_02005654(*(short *)(base + 0x4a));
        break;
    case 7:
    case 8:
    case 9:
        break;
    }
    *(short *)(base + 0x4a) = key;
    *(int *)(base + 0x58) = src[0];
    if (pan < 0) {
        base[0xf2] = ((unsigned char *)src)[4];
    } else {
        if (pan > 0x7f) {
            pan = 0x7f;
        }
        base[0xf2] = pan;
    }
    base[0x60] = base[0xf2];
    base[0x61] = ((unsigned char *)src)[5];
    base[0x62] = ((unsigned char *)src)[6];
    base[0x63] = ((unsigned char *)src)[7];
    *(int *)(base + 0x4c) = owner;
    *(int *)(base + 0x5c) = *(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + src[0] * 16 + 0xc);
    base[0x48] = 0;
    *(int *)(base + 0xec) = 5;
    base[0xf0] = 0;
    base[0x7d] = 8;
    *(unsigned int *)(base + 0xe8) &= ~4;
    *(int *)(base + 0x84) = 3;
    if (frames != 0) {
        *(int *)(base + 0x70) = frames;
        fade = base + 0x70;
        fade[0xc] = base[0xf2];
        *(int *)(fade + 4) = 0;
        *(int *)(fade + 8) = func_0207c1d8(func_0207b3dc(fade[0xc]), func_0207b424(*(int *)(base + 0x70)));
        *(int *)(fade + 0x10) = 0;
        *(int *)(fade + 0x14) = 3;
        fade[0xd] = 8;
        *(unsigned int *)(base + 0xe8) |= 4;
        base[0x60] = 0;
    }
    return 0;
}

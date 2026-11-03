typedef struct {
    short x;
    short y;
} Pair;

extern int data_0209bf44[];
extern void func_02050394(int value);
extern int func_020503a0(int a, int b);
extern void func_020503c8(short x, short y, short z);
extern void func_020503e4(void);
extern void func_02050320(int value);
extern void func_020505a4(int a, int z);
extern int func_02022ca4(void *owner, int value);
extern void *func_0200eb78(void);

#define PACK(v) ((((v) & 0x80000000) >> 16) | ((v) & 0x7fff))

void func_020503f0(int rx, int ry, int z0, int z1, unsigned short color0, unsigned short color1, unsigned char stripes)
{
    Pair outer[33];
    Pair inner[33];
    int first;
    int second;
    int s;
    unsigned int i;
    unsigned int j;
    unsigned int k;
    int idx;
    int c;
    int a;
    int b;
    int e;
    int f;

    func_02050394(2);
    for (i = 0; i < 33; i++) {
        idx = i * 2;
        if (idx >= 0x40) {
            idx -= 0x40;
        }
        c = data_0209bf44[idx + 1];
        a = func_020503a0((ry >> 2) * 3, c);
        s = data_0209bf44[idx];
        b = func_020503a0(ry, s);
        e = func_020503a0((rx >> 2) * 3, c);
        f = func_020503a0(rx, s);
        outer[i].x = PACK(a);
        outer[i].y = PACK(e);
        inner[i].x = PACK(b);
        inner[i].y = PACK(f);
    }
    first = func_02022ca4(func_0200eb78(), color1);
    second = func_02022ca4(func_0200eb78(), color0);
    if (stripes != 0) {
        for (j = 0; j < 33; j++) {
            if (!(j & 1)) {
                func_02050320(first);
                func_020505a4(0, z0);
                func_020503c8(outer[j].x, inner[j].x, 0);
                func_02050320(second);
                func_020505a4(0, z1);
                func_020503c8(outer[j].y, inner[j].y, 0);
            } else {
                func_02050320(first);
                func_020505a4(0, z1);
                func_020503c8(outer[j].x, inner[j].x, 0);
                func_02050320(second);
                func_020505a4(0, z0);
                func_020503c8(outer[j].y, inner[j].y, 0);
            }
        }
    } else {
        for (k = 0; k < 33; k++) {
            func_02050320(first);
            func_020505a4(0, z0);
            func_020503c8(outer[k].x, inner[k].x, 0);
            func_02050320(second);
            func_020505a4(0, z1);
            func_020503c8(outer[k].y, inner[k].y, 0);
        }
    }
    func_020503e4();
}

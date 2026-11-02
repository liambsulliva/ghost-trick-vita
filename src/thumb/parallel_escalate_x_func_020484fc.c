typedef struct Spark020484fc {
    unsigned char active;
    char pad1[3];
    int x;
    int y;
    int size;
    int life;
} Spark020484fc;

extern void *func_02008eb0(void);
extern unsigned int func_01ff80b8(void *random);
extern void *func_02008444(void);
extern int func_0200f5c8(void *camera);
extern int func_0200f5e8(void *camera);

void func_020484fc(unsigned char *owner, Spark020484fc *spark, int low)
{
    unsigned int r;

    spark->life = (func_01ff80b8(func_02008eb0()) % 48) << 12;
    spark->size = (func_01ff80b8(func_02008eb0()) % 16 + 16) << 12;
    spark->active = 1;
    if (owner[0xc4] == 0) {
        r = func_01ff80b8(func_02008eb0()) % 320;
        spark->x = ((r - 160) << 12) + func_0200f5c8(func_02008444()) + 0x80000;
    } else if (owner[0xc4] == 1) {
        r = func_01ff80b8(func_02008eb0()) % 160;
        spark->x = (r << 12) + func_0200f5c8(func_02008444()) + 0x80000;
    } else {
        r = func_01ff80b8(func_02008eb0()) % 160;
        spark->x = ((r - 160) << 12) + func_0200f5c8(func_02008444()) + 0x80000;
    }
    if (low != 0) {
        r = func_01ff80b8(func_02008eb0()) % 256;
        spark->y = ((r - 128) << 12) + func_0200f5e8(func_02008444()) + 0x60000;
    } else {
        r = func_01ff80b8(func_02008eb0());
        spark->y = -((r % 128) << 12) + func_0200f5e8(func_02008444()) + 0x60000;
    }
}

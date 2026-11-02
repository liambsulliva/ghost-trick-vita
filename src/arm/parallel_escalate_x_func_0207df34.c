typedef struct Range0207df34 {
    char pad00[0x14];
    int f14;
    int f18;
    char pad1c[0x1c];
    int f38;
    char pad3c[0xc];
    int f48;
    char pad4c[0x14];
    unsigned int f60;
    unsigned int f64;
    unsigned short f68;
    unsigned char f6a;
    unsigned char f6b;
    unsigned char f6c;
} Range0207df34;

typedef struct Cursor0207df34 {
    int f0;
    const unsigned char *p;
} Cursor0207df34;

extern const unsigned char *func_0207c6f8(const unsigned char *p, unsigned int *out);

void func_0207df34(Range0207df34 *range, Cursor0207df34 *cursor)
{
    const unsigned char *p;
    unsigned int b;

    p = cursor->p;
    b = p[0];
    range->f6b = (b & 0x40) != 0;
    range->f6a = (b & 0x20) != 0;
    range->f6c = (b & 0x80) != 0;
    range->f68 = p[1] << 4;
    range->f68 |= 0x4000;
    p = func_0207c6f8(p + 2, &range->f60);
    if (b & 0x40) {
        p = func_0207c6f8(p, &range->f64);
    }
    if (b & 0x40) {
        if (b & 0x80) {
            range->f18 = range->f38;
        } else {
            range->f18 = range->f48;
        }
    } else {
        range->f18 = range->f14;
    }
}

typedef struct Game03340 {
    char pad[8];
    unsigned int depth;
} Game03340;

extern Game03340 data_020b18a0;
extern unsigned char *data_020b18b8[8];

unsigned char *func_02003340(unsigned char *p, int base)
{
    unsigned char c;
    unsigned char *q;

    c = *p;
    switch (c) {
    case 0x80:
        p++;
        while (*p & 0x80) {
            p++;
        }
        return p + 1;
    case 0x81:
        q = p + 1;
        while (*q & 0x80) {
            q++;
        }
        return q + 1;
    case 0x93:
        return p + 5;
    case 0x94:
        return p + 4;
    case 0x95:
        data_020b18b8[data_020b18a0.depth++] = p + 4;
        p = (unsigned char *)(base + (p[1] | p[2] << 8 | p[3] << 16));
        if (data_020b18a0.depth >= 8) {
            return 0;
        }
        return p;
    case 0xfd:
        return data_020b18b8[--data_020b18a0.depth];
    case 0xfc:
    case 0xff:
        return p + 1;
    case 0xfe:
        return p + 3;
    }
    if (c <= 0x7f) {
        q = p + 2;
        while (*q & 0x80) {
            q++;
        }
        return q + 1;
    }
    if (c >= 0xa0 && c <= 0xa2) {
        return p + 1;
    }
    if (c >= 0xc0 && c <= 0xd6) {
        return p + 2;
    }
    if (c >= 0xe0 && c <= 0xe3) {
        return p + 3;
    }
    if (c >= 0xb0 && c <= 0xbd) {
        return p + 4;
    }
    return 0;
}

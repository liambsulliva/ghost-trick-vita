#pragma opt_propagation off
typedef struct Info03138 {
    unsigned short mask;
    unsigned short wait;
    unsigned short offs[16];
} Info03138;

typedef struct State03138 {
    int pad0;
    int pad4;
    int busy;
} State03138;

extern State03138 data_020b18a0;
extern unsigned char *func_02003340(unsigned char *p, int base);

static inline void Finish(int *d)
{
    *d = 1;
}

static inline void Clear(int *d)
{
    *d = 0;
}

void func_02003138(unsigned char *base, int offset, Info03138 *info)
{
    int done; int seen; unsigned char *p;

    p = base + offset;
    seen = 0;
    done = 0;
    info->wait = 0x78;
    info->mask = 1;
    data_020b18a0.busy = seen;
    while (done == 0) {
        if (*p == 0xff) {
            done = 1;
        } else if (*p == 0xfe) {
            info->mask |= (unsigned short)(p[1] | (p[2] << 8));
            p += 3;
        } else if (*p == 0x93) {
            { int i; i = p[1]; info->offs[i] = (p[2] | (p[3] << 8)) - offset; }
            p += 5;
        } else if (*p == 0xe1) {
            if (seen == 0) {
                { int h; int l; h = p[2]; l = p[1]; seen = 1; info->wait = (h << 8) | l; }
            }
            p += 3;
        } else {
            p = func_02003340(p, (int)base);
            if (p == 0) {
                done = 1;
            }
        }
    }
}

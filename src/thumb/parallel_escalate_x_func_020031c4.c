typedef struct Game031c4 {
    char pad[8];
    int f8;
} Game031c4;

typedef struct Out031c4 {
    signed char a;
    signed char b;
    unsigned short c;
} Out031c4;

extern Game031c4 data_020b18a0;
extern unsigned char *func_02003340(unsigned char *p, int arg);

unsigned char *func_020031c4(unsigned char *p, int arg, Out031c4 *out)
{
    int fa;
    int fb;
    int done;
    unsigned char c;
    unsigned char *q;

    fa = 0;
    fb = 0;
    done = 0;
    out->a = 0x7f;
    out->b = 0x40;
    out->c = 0;
    data_020b18a0.f8 = 0;
    while (!done) {
        c = *p;
        if (c == 0xff) {
            done = 1;
        } else if (c == 0xc1) {
            p = (unsigned char *)((int)p + 1);
            if (fa == 0) {
                out->a = *(signed char *)p;
                fa = 1;
            }
            p++;
        } else if (c == 0xc0) {
            q = p + 1;
            if (fb == 0) {
                out->b = *(signed char *)q;
                fb = 1;
            }
            p = q + 1;
        } else {
            p = func_02003340(p, arg);
            if (p == 0) {
                done = 1;
            }
        }
    }
    return p;
}

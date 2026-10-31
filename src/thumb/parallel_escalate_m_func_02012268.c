extern int func_02008444();
extern int func_02010194();
extern void func_02058a80();
extern int func_0200f190();
extern int func_0201230c();
extern void func_020082d8();

int func_02012268(unsigned char *p)
{
    int v[3];
    int w[3];
    int obj;
    int y;
    unsigned int i;
    int cur;
    int tgt;

    if (p[4] != 0) {
        return 0;
    }
    obj = func_02010194(func_02008444());
    if (obj != 0) {
    func_02058a80(v, obj);
    y = v[1] >> 12;
    for (i = 0; i < 4; i++) {
        if (*(unsigned short *)(p + i * 2 + 6) > y && *(unsigned short *)(p + i * 2 + 0xe) < y) {
            if (*(unsigned short *)((int)p + 6 + i * 2) - 0x10 < y) {
                return 0;
            }
            cur = func_0200f190(func_02008444()) + 0xc0000;
            tgt = *(unsigned short *)((int)p + 6 + i * 2) << 12;
            func_020082d8(w, func_0201230c(func_02008444()));
            tgt = tgt + w[1];
            if (cur != tgt) {
                if (tgt < cur) {
                    if (cur - tgt > 0x4000) {
                        return -0x4000;
                    }
                    return tgt - cur;
                }
                if (tgt - cur > 0x4000) {
                    return 0x4000;
                }
                return tgt - cur;
            }
            return 0;
        }
    }
    }
    return 0;
}

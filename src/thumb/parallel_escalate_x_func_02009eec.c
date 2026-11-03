typedef int code();

extern void func_0200a478(void);
extern void func_02007bd8(void);
extern int *func_02007b70(void);
extern void func_02009a0c(unsigned char *buf, int size);
extern void func_0200a874(unsigned char *slot, unsigned char *buf);
extern void func_0200a644(unsigned char *obj, unsigned char id);

void func_02009eec(unsigned char *p)
{
    unsigned char *base2;
    unsigned char *base1;
    unsigned char *buf;
    unsigned int w;
    unsigned int h;
    unsigned int y;
    unsigned int x;
    unsigned int i;
    unsigned char *a;
    unsigned char *b;
    unsigned char *c;
    int *mgr;

    func_0200a478();
    func_02007bd8();
    mgr = func_02007b70();
    buf = (unsigned char *)(*(code **)(*mgr + 0xc))(mgr, 0xc00);
    if (buf != 0) {
        func_02009a0c(buf, 0xc00);
        for (i = 1; i < 0x10; i++) {
            func_0200a874(p + i * 0x10, buf);
        }
        w = (unsigned int)*(unsigned short *)(p + 0xaa28) >> 4;
        h = (unsigned int)*(unsigned short *)(p + 0xaa2a) >> 4;
        y = 0;
        base1 = p + 0x6202;
        base2 = p + 0x6e02;
        for (; y < h; y++) {
            a = base1 + y * 0x40;
            b = base2 + y * 0x40;
            c = buf + y * 0x40;
            x = 0;
            if (x < w) {
                for (; x < w; x++, a++, b++, c++) {
                    if (*b != *c) {
                        func_0200a644(p + 0x100, *a);
                        *a = 0;
                        *b = *c;
                    }
                }
            }
        }
        p[0xaa40] = 1;
        p[0xaa3c] = 0xff;
        p[0xaa3d] = 0xff;
    }
}

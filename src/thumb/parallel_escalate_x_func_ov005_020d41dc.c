struct Words5 {
    int v[5];
};

struct Shorts5 {
    short v[5];
};

struct Codes5 {
    unsigned short v[5];
};

extern const struct Words5 data_ov005_020d478c;
extern const struct Words5 data_ov005_020d47a0;
extern const struct Words5 data_ov005_020d47b4;
extern const struct Words5 data_ov005_020d47c8;
extern const struct Codes5 data_ov005_020d4780;
extern const struct Codes5 data_ov005_020d4774;
extern const struct Codes5 data_ov005_020d475c;
extern const struct Shorts5 data_ov005_020d4750;
extern const struct Shorts5 data_ov005_020d4768;
extern const struct Shorts5 data_ov005_020d4738;
extern const struct Shorts5 data_ov005_020d4744;
extern void *func_0202f624(int size);
extern void *func_02067c88(void *part, int a, int b, int c, int d, int e, int f, int g);
extern void func_02067d80(void *part);
extern void func_02067da8(void *part);
extern void func_02014ddc(void *part, int x);
extern void func_02014de4(void *part, int y);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, void *part, int a, int b);

void *func_ov005_020d41dc(void *owner, int kind, int mode, int flag)
{
    struct Words5 a;
    struct Words5 b;
    struct Words5 c;
    struct Words5 d;
    struct Codes5 e;
    struct Codes5 f;
    struct Codes5 g;
    struct Shorts5 x2;
    struct Shorts5 y2;
    struct Shorts5 x1;
    struct Shorts5 y1;
    void *part;

    a = data_ov005_020d478c;
    b = data_ov005_020d47a0;
    c = data_ov005_020d47b4;
    d = data_ov005_020d47c8;
    e = data_ov005_020d4780;
    f = data_ov005_020d4774;
    g = data_ov005_020d475c;
    x2 = data_ov005_020d4750;
    y2 = data_ov005_020d4768;
    x1 = data_ov005_020d4738;
    y1 = data_ov005_020d4744;
    part = 0;
    if (kind == 0 || mode == 1 || kind == 3) {
        if (kind == 4 && flag == 0) {
            return 0;
        }
        if (kind == 0 && mode != 0) {
            part = func_0202f624(0xe8);
            if (part != 0) {
                part = func_02067c88(part, a.v[kind], b.v[kind], c.v[kind], d.v[kind], 0x807a, 0x807b, 0x807c);
            }
        } else {
            part = func_0202f624(0xe8);
            if (part != 0) {
                part = func_02067c88(part, a.v[kind], b.v[kind], c.v[kind], d.v[kind], e.v[kind], f.v[kind], g.v[kind]);
            }
        }
        func_02067d80(part);
        if (mode == 1) {
            func_02014ddc(part, x1.v[kind] << 12);
            func_02014de4(part, y1.v[kind] << 12);
            if (kind == 1) {
                func_02067da8(part);
            }
        } else {
            func_02014ddc(part, x2.v[kind] << 12);
            func_02014de4(part, y2.v[kind] << 12);
            if (kind == 0) {
                func_02067da8(part);
            }
        }
        func_0202f7e8(func_02009900(), part, 1, 0);
    }
    return part;
}

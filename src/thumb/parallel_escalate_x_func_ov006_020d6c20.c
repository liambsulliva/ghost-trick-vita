struct Vec {
    int x;
    int y;
    int z;
};

struct Pair {
    int a;
    int b;
};

struct Pairs2 {
    struct Pair e[2];
};

struct Info {
    int count;
    int pad4;
};

struct Infos2 {
    struct Info *p[2];
};

struct Shorts5 {
    short v[5];
};

struct Codes4 {
    unsigned short v[4];
};

struct Page {
    char pad0[0x20];
    int mode;
    char pad24;
    unsigned char page;
    char pad26[0xc];
    signed char sel[2];
    char pad34[4];
    signed char rows[2][50];
    char pad9c[0x190];
    int marks[2][50];
    char pad3bc[0x5c];
    void *frame_top;
    void *frame_bottom;
    void *arrow_left;
    void *arrow_right;
    void *hint;
    void *tab_left;
    void *tab_right;
    char pad434[0x214];
    void *icons[5];
};

extern const struct Pairs2 data_ov006_020d8c84;
extern const struct Infos2 data_ov006_020d8a50;
extern const struct Shorts5 data_ov006_020d8b1c;
extern const struct Codes4 data_ov006_020d8ab0;
extern char data_020ce17c[];
extern void func_ov006_020d8014(void *frame, int a, int b, int c, int d);
extern void func_ov006_020d8050(void *frame, int x, int y);
extern void *func_ov006_020d809c(int a, int b, int c, int d, int e, int f, int g);
extern void *func_ov006_020d813c(int a, int b, int c);
extern int func_ov006_020d74c8(struct Page *p, int side, int row);
extern void func_02014de4(void *object, int value);
extern void func_ov006_020d6f80(void *object, int value);
extern void func_0202f64c(void *slot);
extern void func_ov006_020d7ff4(void *object);
extern void *func_0203ec54(char *type);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, void *object, int a, int b);
extern void func_02008398(struct Vec *v, int x, int y, int z);
extern void func_020083a4(void *object, struct Vec *v);
extern void func_ov006_020d7cf8(struct Page *p, int pos, int limit);
extern void func_ov006_020d7838(struct Page *p, int a, int b, int c, int d, int e);

void func_ov006_020d6c20(struct Page *p)
{
    struct Pairs2 tabs;
    struct Shorts5 xs;
    struct Vec pos;
    struct Infos2 infos;
    struct Codes4 codes;
    unsigned char side;
    signed char row;
    signed char at;
    int limit;
    unsigned int i;

    side = p->page & 1;
    tabs = data_ov006_020d8c84;
    if (p->page < 2) {
        func_ov006_020d8014(p->frame_top, -0x80, -0xc, -0x68, 0xc);
        func_ov006_020d8050(p->frame_top, 0x80, 0x60);
        func_ov006_020d8014(p->frame_bottom, 0x68, -0xc, 0x80, 0xc);
        func_ov006_020d8050(p->frame_bottom, 0x80, 0x60);
        row = p->rows[side][p->sel[side]];
        if (p->hint == 0 && func_ov006_020d74c8(p, side, row) != 0 && p->mode == 0) {
            p->hint = func_ov006_020d813c(0x8008, 3, 2);
            func_02014de4(p->hint, 0x5e000);
        }
        if (p->tab_left == 0) {
            p->tab_left = func_ov006_020d813c(-1, 6, 2);
        }
        if (p->tab_right == 0) {
            p->tab_right = func_ov006_020d813c(-1, 6, 2);
        }
        func_ov006_020d6f80(p->tab_left, tabs.e[side].a);
        func_ov006_020d6f80(p->tab_right, tabs.e[side].b);
        for (i = 0; i < 5; i++) {
            func_0202f64c(&p->icons[i]);
        }
        func_0202f64c(&p->arrow_left);
        func_0202f64c(&p->arrow_right);
    } else {
        func_ov006_020d8014(p->frame_top, -0x80, -0xc, -0x68, 0xc);
        func_ov006_020d8050(p->frame_top, 0x80, 0xc);
        func_ov006_020d8014(p->frame_bottom, 0x68, -0xc, 0x80, 0xc);
        func_ov006_020d8050(p->frame_bottom, 0x80, 0xc);
        if (p->mode == 1) {
            if (p->arrow_left == 0) {
                p->arrow_left = func_ov006_020d809c(0x75, 0x76, 0x77, -0x10, -0x10, 0x10, 0x10);
            }
            func_ov006_020d8050(p->arrow_left, 0x80, 0xb0);
            if (p->arrow_right == 0) {
                p->arrow_right = func_ov006_020d809c(0x72, 0x73, 0x74, -0x10, -0x10, 0x10, 0x10);
            }
            func_ov006_020d8050(p->arrow_right, 0xf0, 0xb0);
            infos = data_ov006_020d8a50;
            row = p->rows[side][p->sel[p->page & 1]];
            limit = infos.p[p->page & 1][row].count;
            at = p->marks[p->page & 1][row];
            if (at <= 0) {
                func_ov006_020d7ff4(p->arrow_left);
            }
            if (at >= limit - 1) {
                func_ov006_020d7ff4(p->arrow_right);
            }
            xs = data_ov006_020d8b1c;
            for (i = 0; i < 5; i++) {
                if (p->icons[i] == 0) {
                    p->icons[i] = func_0203ec54(data_020ce17c);
                    func_0202f7e8(func_02009900(), p->icons[i], 1, 0);
                    if (i == 4) {
                        func_ov006_020d6f80(p->icons[i], 0xba);
                    }
                }
                func_02008398(&pos, xs.v[i] << 12, 0xb0000, 0);
                func_020083a4(p->icons[i], &pos);
            }
            func_ov006_020d7cf8(p, at, limit);
        } else {
            func_0202f64c(&p->arrow_left);
            func_0202f64c(&p->arrow_right);
        }
        func_0202f64c(&p->tab_left);
        func_0202f64c(&p->tab_right);
        func_0202f64c(&p->hint);
    }
    codes = data_ov006_020d8ab0;
    func_ov006_020d7838(p, codes.v[p->page] + 0x3e, 0, 0x15, 0x40, 0);
    func_ov006_020d7838(p, codes.v[p->page] * 2 + 0x12e, 2, 0x19, 0, 0);
    func_ov006_020d7838(p, codes.v[p->page] * 2 + 0x12f, 2, 0x1d, 0, 2);
}

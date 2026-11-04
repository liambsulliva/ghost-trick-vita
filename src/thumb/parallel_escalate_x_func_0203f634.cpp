class Res {
public:
    virtual void s0();
    virtual void s1();
    virtual void *s2(int size, void *name);
    virtual void *s3(int size);
};

class Screen {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    unsigned char pad04[0x24];
    unsigned char sub28[0x94];
    int fbc;
    int fc0;
    int fc4;
    struct Tile *tiles;
};

struct Tile {
    int a;
    int b;
};

struct Image {
    unsigned short w;
    unsigned short h : 15;
    unsigned short flag : 1;
    unsigned char pad4[0x1fc];
    unsigned char pal[0x200];
};

struct Name {
    unsigned short *id;
    int pad;
};

extern "C" {
extern const struct Name data_0208332c[];
extern char data_0209af5c;
extern void func_0200b518(void *object, int a, unsigned int b, unsigned int c);
extern void func_0200b598(void *object, int a, int b);
extern void func_020280a8(void *object, unsigned char a);
extern void *func_0200883c(void);
extern void *func_02008824(void *a);
extern struct Image *func_0201e888(void *object, int a, unsigned int b, int c, int d);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *a);
extern void func_0207303c(void *src, void *dst, int size);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *a, void *buf, int b, int c, int d);
extern void func_02072fa8(void *dst, int value, int size);
extern void func_0203f83c(void);
extern int func_0203f93c(Screen *p);
extern void func_0203f850(struct Tile *tile, int x, int y, int a, int b, int c, int d, unsigned int e, int f, int g, int h, int i);
extern void func_0203f91c(Screen *p);
extern void *func_02008860(void);
extern void func_0203f910(void *a);

void func_0203f634(Screen *p)
{
    int index;
    struct Image *image;
    unsigned char *pal;
    unsigned char *buf;
    unsigned char *a;
    int off;
    int n;
    unsigned int i;
    unsigned int j;
    unsigned char *b;
    unsigned int row;
    int w;
    int y;
    int size;

    if (p->fbc != 0) {
        func_0200b518(p->sub28, 1, 0x80000000, 0x80000000);
        func_0200b598(p->sub28, p->fbc, 0xf);
        func_020280a8(p->sub28, p->fc4);
        p->s10();
        return;
    }
    index = p->fc0;
    if (index == 0) {
        return;
    }
    image = func_0201e888(func_02008824(func_0200883c()), 4, *data_0208332c[index].id, 1, 0);
    if (image == 0) {
        return;
    }
    buf = (unsigned char *)func_02007b70(func_02007bd8())->s3(0x200);
    func_0207303c(image->pal, buf, 0x200);
    func_0202f1bc(func_02009c1c(), buf, 0, 0x200, 0x1f);
    size = image->w * image->h;
    a = (unsigned char *)func_02007b70(func_02007bd8())->s3(size);
    b = (unsigned char *)func_02007b70(func_02007bd8())->s3(size);
    if (a != 0) {
        pal = image->pal;
        func_0207303c(pal + 0x200, b, size);
        off = 0;
        for (row = 0; row < 0x60; row++) {
            func_0207303c(b + off, a + off, 0x80);
            func_0207303c(b + off + 0x80, a + off + 0x100, 0x80);
            func_0207303c(b + off + 0x100, a + off + 0x80, 0x80);
            func_0207303c(b + off + 0x180, a + off + 0x180, 0x80);
            off += 0x200;
        }
        func_0202f1bc(func_02009c1c(), a, 0x1800, size, 0x13);
    }
    p->tiles = (struct Tile *)func_02007b70(func_02007bd8())->s2(0x300, &data_0209af5c);
    func_02072fa8(p->tiles, 0xc0, 0x300);
    func_0203f83c();
    n = 0;
    w = func_0203f93c(p);
    for (i = 0; i < 0xc; i++) {
        j = 0;
        y = i * 16;
        for (; j < 8; j++) {
            func_0203f850(&p->tiles[i * 8 + j], j * 32, y, 1, 0, 0, 0, 0x80004000, 1, n * w + 0x30, 0, 0);
            n++;
        }
    }
    func_0203f91c(p);
    func_0203f910(func_02008860());
}
}

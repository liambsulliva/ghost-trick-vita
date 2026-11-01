struct Point4ba14 { int x; int y; };
struct Color4ba14 { unsigned char r; unsigned char g; unsigned char b; int alpha; };

struct Sprite4ba14 {
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
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33(int a);
    virtual void s34();
    virtual void s35(int a);
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int a, int b, int c, int d, int e, int f, int g);
};

struct Src4ba14 {
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(Point4ba14 *a, void *b, int c, int d);
};

struct Obj4ba14 {
    char pad00[0xd8];
    Sprite4ba14 *sprite;
    short id;
    char padde;
    unsigned char ready;
    unsigned short frame;
    char pade2[2];
    char area[4];
};

extern "C" {
int func_02047914(Src4ba14 *a);
int func_02044d74(Sprite4ba14 *a);
int func_02044d78(Sprite4ba14 *a);
void func_0205765c(Sprite4ba14 *a, int b, int c, int d, int e);
void func_0202e46c(Point4ba14 *a);
void func_0202c700(Sprite4ba14 *a, unsigned int b, Point4ba14 *c);
void *func_02047b38(Src4ba14 *a);
void func_02047b3c(Color4ba14 *a, void *b);
void func_0200ee34(Sprite4ba14 *a, unsigned int b, unsigned int c, unsigned int d);
}

extern "C" int func_0204ba14(Obj4ba14 *p, Src4ba14 *src, int arg2, int arg3, unsigned short *counter)
{
    Point4ba14 pts[4];
    Color4ba14 color;
    unsigned int i;
    int w;

    if (p->id == func_02047914(src)) {
        if (p->ready == 0) {
            p->sprite->s42((unsigned short)(p->frame * 5 + 0x417), 0, 1, 0, 1, -1, 0);
            w = func_02044d74(p->sprite);
            func_0205765c(p->sprite, 0, 0, w, func_02044d78(p->sprite));
            p->ready = 1;
        }
        func_0202e46c(&pts[0]);
        func_0202e46c(&pts[1]);
        func_0202e46c(&pts[2]);
        func_0202e46c(&pts[3]);
        src->s8(pts, p->area, arg2, arg3);
        for (i = 0; i < 4; i++) {
            func_0202c700(p->sprite, i, &pts[i]);
        }
        func_02047b3c(&color, func_02047b38(src));
        p->sprite->s33(*counter);
        *counter = *counter - 1;
        func_0200ee34(p->sprite, color.r, color.g, color.b);
        p->sprite->s35((signed char)((color.alpha * 31 / 100) >> 12));
        return 1;
    }
    return 0;
}

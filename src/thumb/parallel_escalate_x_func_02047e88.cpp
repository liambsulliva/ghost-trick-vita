class Sprite {
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
    virtual void s33(unsigned short a);
    virtual void s34();
    virtual void s35(signed char a);
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(unsigned short a, int b, int c, int d, int e, int f, int g);
};

struct Rect {
    short x;
    short y;
    short w;
    short h;
};

class Source {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(struct Rect *rect, void *area, int a, int b);
};

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char pad;
    int alpha;
};

struct Window {
    char pad00[0xd8];
    Sprite *main;
    short id;
    char pade[1];
    unsigned char ready;
    unsigned char index;
    char pade1[7];
    Sprite *sprites[6];
    short ids[6];
    char pad10c[6];
    unsigned char loaded[6];
    unsigned char kinds[6];
    char pad11e[2];
    char area[0x34];
};

extern "C" {
extern unsigned short data_0209b7b4[];
extern short func_02047914(Source *source);
extern int func_02044d74(Sprite *sprite);
extern int func_02044d78(Sprite *sprite);
extern void func_0205765c(Sprite *sprite, int a, int b, int c, int d);
extern void func_0202e46c(struct Rect *rect);
extern void func_0202c700(Sprite *sprite, unsigned int i, struct Rect *rect);
extern int func_02047b38(Source *source);
extern void func_02047b3c(struct Color *color, int value);
extern void func_0200ee34(Sprite *sprite, unsigned char r, unsigned char g, unsigned char b);
int func_02047e88(struct Window *window, Source *source, int a, int b, unsigned short *base);
}

int func_02047e88(struct Window *window, Source *source, int a, int b, unsigned short *base)
{
    struct Rect rects[4];
    struct Rect parts[4];
    struct Color color;
    struct Color tint;
    unsigned int i;
    unsigned int j;
    int w;
    Sprite **slot;

    if (window->id == func_02047914(source)) {
        if (window->ready == 0) {
            window->main->s42(data_0209b7b4[window->index], 0, 1, 0, 1, -1, 0);
            w = func_02044d74(window->main);
            func_0205765c(window->main, 0, 0, w, func_02044d78(window->main));
            window->ready = 1;
        }
        func_0202e46c(&rects[0]);
        func_0202e46c(&rects[1]);
        func_0202e46c(&rects[2]);
        func_0202e46c(&rects[3]);
        source->s8(rects, window->area, a, b);
        for (i = 0; i < 4; i++) {
            func_0202c700(window->main, i, &rects[i]);
        }
        func_02047b3c(&color, func_02047b38(source));
        window->main->s33(*base);
        (*base)--;
        func_0200ee34(window->main, color.r, color.g, color.b);
        window->main->s35((signed char)((color.alpha * 31 / 100) >> 12));
        return 1;
    }
    for (j = 0; j < 6; j++) {
        if (window->ids[j] == func_02047914(source)) {
            if (j == 1 && window->kinds[j] == 0) {
                window->sprites[j]->s35(0);
                return 1;
            }
            if (window->loaded[j] == 0) {
                window->sprites[j]->s42(window->kinds[j] + 0xb1, 0, 1, 0, 1, -1, 0);
                w = func_02044d74(window->sprites[j]);
                func_0205765c(window->sprites[j], 0, 0, w, func_02044d78(window->sprites[j]));
                window->loaded[j] = 1;
            }
            func_0202e46c(&parts[0]);
            func_0202e46c(&parts[1]);
            func_0202e46c(&parts[2]);
            func_0202e46c(&parts[3]);
            source->s8(parts, window->area, a, b);
            for (i = 0; i < 4; i++) {
                func_0202c700(window->sprites[j], i, &parts[i]);
            }
            func_02047b3c(&tint, func_02047b38(source));
            slot = &window->sprites[j];
            (*slot)->s33(*base);
            (*base)--;
            func_0200ee34(*slot, tint.r, tint.g, tint.b);
            (*slot)->s35((signed char)((tint.alpha * 31 / 100) >> 12));
            return 1;
        }
    }
    return 0;
}

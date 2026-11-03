struct Layout {
    char pad[0x54];
};

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
    virtual void s15(int y);
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
    virtual void s33(int value);
    virtual void s34();
    virtual void s35(signed char color);
    virtual void s36();
    virtual void s37(int value);
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44(struct Layout *layout);
    virtual void s45();
    virtual void s46(int x, int y);
    char pad[0xc4];
};

struct StripBits {
    unsigned char b0 : 1;
    unsigned char ready : 1;
};

struct Strip {
    char pad0[0x28];
    Sprite *items;
    char pad2c[8];
    struct Layout layout;
    struct StripBits bits;
    char pad89;
    unsigned char index;
    char pad8b;
    unsigned short last;
    unsigned short count;
    char pad90[0x10];
    unsigned char flags;
    char pada1;
    unsigned short a2;
    char pada4[4];
    int vram;
    short widths[8];
    unsigned short ids[8];
};

extern "C" {
extern void func_0207dea4(void *array, int size, int count, void *dtor);
extern void *func_0207dc50(int count, int size, int align, void *ctor, void *dtor);
extern void *func_02056b2c(void *object);
extern void *func_02056c8c(void *object);
extern int func_02019a58(void *table, unsigned int index);
extern unsigned char func_020556bc(void *table, int index);
extern unsigned short func_020556c8(void *table, int index);
extern void func_02042b20(struct Layout *layout);
extern int func_02019cd8(void *table);
extern void func_02042820(struct Layout *layout, int vram, int size, int a, int b, int c, int d, int e);
extern char *func_020556d4(void *table);
extern void func_020428f0(struct Layout *layout, char *frame, int size, int a, int b, int c, int d, int e, int f);
extern void func_02029014(struct Layout *layout, int color);
extern void func_02029018(struct Layout *layout, int color);
extern void func_0202901c(struct Layout *layout, unsigned char font);
extern int func_02019cdc(void *table, unsigned int index, unsigned short value);
extern void func_0205765c(Sprite *sprite, int a, int b, int c, int d);
extern unsigned short func_020556d8(void *table, int index, unsigned short value);
void func_02055444(struct Strip *strip, void *table);
}

void func_02055444(struct Strip *strip, void *table)
{
    unsigned short i;
    unsigned short j;
    unsigned char color;
    int start;
    int end;
    int base;
    short width;

    if (strip->items != 0) {
        func_0207dea4(strip->items, 200, 8, (void *)func_02056c8c);
        strip->items = 0;
    }
    strip->count = func_02019a58(table, strip->index);
    strip->items = (Sprite *)func_0207dc50(strip->count, 200, 8, (void *)func_02056b2c, (void *)func_02056c8c);
    strip->last = 0;
    strip->flags = func_020556bc(table, strip->index);
    strip->a2 = func_020556c8(table, strip->index);
    func_02042b20(&strip->layout);
    base = func_02019cd8(table);
    strip->vram = base + strip->index * 0x6000;
    strip->bits.ready = 0;
    func_02042820(&strip->layout, strip->vram, 0x6000, 3, 2, 0, 0xffff, -1);
    func_020428f0(&strip->layout, func_020556d4(table), 0x20, 3, 0, 0xffff, 0, -1, 0x100);
    func_02029014(&strip->layout, 5);
    func_02029018(&strip->layout, 5);
    func_0202901c(&strip->layout, 3);
    color = 0;
    if (strip->index == 0 && (strip->flags & 1)) {
        color = 0x1f;
    }
    for (i = 0; i < strip->count; i++) {
        start = func_02019cdc(table, strip->index, i + 1);
        end = 0xc0;
        if (i < strip->count) {
            end = func_02019cdc(table, strip->index, i + 2);
        }
        strip->items[i].s44(&strip->layout);
        func_0205765c(&strip->items[i], 0, start << 12, 0x100000, end << 12);
        strip->items[i].s46(0x100000, (end - start) << 12);
        strip->items[i].s37(0x38);
        strip->items[i].s33(0x450);
        strip->items[i].s35(color);
        strip->items[i].s15((0x60 - ((start + end) >> 1)) << 12);
        strip->items[i].s11();
        strip->ids[i] = func_020556d8(table, strip->index, i + 1);
    }
    if (strip->count <= 1) {
        strip->widths[0] = 0x1f;
    } else {
        width = (strip->count * 0x1f - 0x32) / (strip->count - 1);
        for (j = 0; j < strip->count - 1; j++) {
            strip->widths[j] = width;
        }
        strip->widths[strip->count - 1] = 0x32;
    }
}

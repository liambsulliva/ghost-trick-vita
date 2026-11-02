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
    virtual void s13(int x, int y, int z);
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
    virtual void s33(int value);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int id, int a, int b, int c, int d, int e, int f);
    virtual void s43();
    virtual void s44(void *label);
    virtual void s45();
    virtual void s46(int a, int b);
};

class Res {
public:
    virtual void r0();
    virtual void r1();
    virtual void *r2(int size, void *name);
    virtual void r3();
    virtual void r4(void *ptr);
};

struct LabelInfo {
    unsigned char pad0[0x10];
    int w10;
};

struct Owner {
    unsigned char pad0[0x28];
    Sprite *bar;
    Sprite *frame;
    Sprite *icon;
    unsigned char pad34[4];
    unsigned char label[0x54];
    void *text;
    unsigned char pad90[0xd];
    signed char kind;
};

extern "C" {
extern void *func_0202f624(int size);
extern Sprite *func_0205a7a0(void *mem);
extern void func_0205765c(Sprite *obj, int a, int b, int c, int d);
extern int func_0205b830(Owner *p);
extern int func_0205b838(Owner *p);
extern int func_0205b850(Owner *p);
extern int func_0205b858(Owner *p);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, Sprite *obj, int a, int b);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *system);
extern void func_02042b20(void *label);
extern void *func_0200883c(void);
extern void *func_0201a4e8(void *game);
extern void *func_0201e888(void *font, int a, int id, int b, int c);
extern void func_020427b0(void *label, void *text, int a, int b, int c, int d, int e, int f, int g, int h, int i);
extern void *func_02043fa0(void *label);
extern struct LabelInfo *func_02042d18(void *label);
extern void func_0205b034(void *a, int b);
extern void *func_02008444(void);
extern void *func_0205b3a4(void *game, signed char kind);
extern void *func_0201003c(void);
extern int func_0205b040(void *entry);
extern void func_02015bf0(void *mgr, int key, struct LabelInfo *info);
void func_0205b59c(Owner *p);
}

void func_0205b59c(Owner *p)
{
    Sprite *obj;

    if (p->bar != 0) {
        return;
    }
    obj = (Sprite *)func_0202f624(0xc8);
    if (obj != 0) {
        obj = func_0205a7a0(obj);
    }
    p->bar = obj;
    if (p->bar != 0) {
        p->bar->s42(0xc0, 0, 1, 0, 1, -1, 0);
        p->bar->s33(0x440);
        func_0205765c(p->bar, 0, 0, 0x100000, 0x30000);
        p->bar->s46(0x100000, 0x30000);
        p->bar->s13(func_0205b830(p) << 12, func_0205b838(p) << 12, 0);
        p->bar->s11();
        func_0202f7e8(func_02009900(), p->bar, 1, 0);
    }
    obj = (Sprite *)func_0202f624(0xc8);
    if (obj != 0) {
        obj = func_0205a7a0(obj);
    }
    p->frame = obj;
    if (p->frame != 0) {
        p->frame->s42(0xc5, 0, 1, 0, 1, -1, 0);
        p->frame->s33(0x420);
        func_0205765c(p->frame, 0, 0, 0x10000, 0x10000);
        p->frame->s46(0x10000, 0x10000);
        p->frame->s13(func_0205b850(p) << 12, func_0205b858(p) << 12, 0);
        p->frame->s11();
        func_0202f7e8(func_02009900(), p->frame, 1, 0);
    }
    if (p->text != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            func_02007b70(func_02007bd8())->r4(p->text);
            p->text = 0;
        }
    }
    func_02042b20(p->label);
    p->text = func_0201e888(func_0201a4e8(func_0200883c()), 0, 0xc3, 0, 0);
    func_020427b0(p->label, p->text, 0, 0, 1, 0, 0xffff, 1, -1, -1, 0x100);
    func_0205b034(func_02043fa0(p->label), func_02042d18(p->label)->w10);
    if (func_0205b3a4(func_02008444(), p->kind) == 0) {
        return;
    }
    func_02015bf0(func_0201003c(), func_0205b040(func_0205b3a4(func_02008444(), p->kind)), func_02042d18(p->label));
    obj = (Sprite *)func_0202f624(0xc8);
    if (obj != 0) {
        obj = func_0205a7a0(obj);
    }
    p->icon = obj;
    if (p->icon == 0) {
        return;
    }
    p->icon->s44(p->label);
    p->icon->s33(0x410);
    func_0205765c(p->icon, 0, 0, 0x100000, 0x30000);
    p->icon->s46(0x100000, 0x30000);
    p->icon->s13(func_0205b830(p) << 12, func_0205b838(p) << 12, 0);
    p->icon->s11();
    func_0202f7e8(func_02009900(), p->icon, 1, 0);
}

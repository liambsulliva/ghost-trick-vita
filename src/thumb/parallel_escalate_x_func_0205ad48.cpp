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

struct Card {
    unsigned char pad0[0x13];
    unsigned char b13;
    unsigned char pad14[0x14];
    unsigned short h28;
    unsigned short h2a;
    short h2c;
    unsigned char pad2e[2];
    unsigned char b30;
    unsigned char b31;
    unsigned char pad32[0xe];
    Sprite *marker;
    void *slots[4];
    unsigned char pad54[0x70];
    Sprite *icon;
    unsigned char label[0x54];
    void *text;
    signed char pick;
};

extern "C" {
extern void *func_0202f624(int size);
extern Sprite *func_0205a7a0(void *mem);
extern void *func_0205b420(void *mem, unsigned char index, int flag);
extern void func_0205765c(Sprite *obj, int a, int b, int c, int d);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, void *obj, int a, int b);
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
extern int func_0205b00c(void *game);
extern void func_0205b014(void *obj, int a, int b);
extern void *func_0205b020(void *game, unsigned int index);
extern void func_0205b030(void *entry, int value);
extern int func_0205b040(void *entry);
extern int func_02011f30(void *entry);
extern void *func_0201003c(void);
extern void func_02015bf0(void *mgr, int key, struct LabelInfo *info);
void func_0205ad48(Card *p);
}

void func_0205ad48(Card *p)
{
    unsigned int i;
    unsigned int j;
    void *obj;
    void *entry;
    Sprite *sprite;
    int x;

    p->h28 = func_0205b00c(func_02008444());
    p->h2a = p->h28;
    if (p->h28 > 0x20) {
        p->h28 = 0x20;
        p->h2a = p->h28;
    }
    if (p->b30 == 0 && p->h28 > 4) {
        p->h28 = 4;
    }
    if (p->h28 > 3) {
        p->h2a = 4;
        p->h2c = 0x20;
    }
    for (i = 0; i < 4; i++) {
        obj = func_0202f624(0xa4);
        if (obj != 0) {
            obj = func_0205b420(obj, i, 0);
        }
        p->slots[i] = obj;
        func_0205b014(p->slots[i], 0, p->h2c);
        func_0202f7e8(func_02009900(), p->slots[i], 1, 0);
    }
    for (j = 0; j < 4; j++) {
        entry = func_0205b020(func_02008444(), j);
        if (entry != 0 && func_02011f30(entry) != 0) {
            p->pick = j;
            func_0205b030(entry, 0);
            p->b31 = 1;
            break;
        }
    }
    if (p->pick != -1) {
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
        func_02015bf0(func_0201003c(), func_0205b040(func_0205b020(func_02008444(), p->pick)), func_02042d18(p->label));
        sprite = (Sprite *)func_0202f624(0xc8);
        if (sprite != 0) {
            sprite = func_0205a7a0(sprite);
        }
        p->icon = sprite;
        if (p->icon != 0) {
            p->icon->s44(p->label);
            p->icon->s33(0x410);
            func_0205765c(p->icon, 0, 0, 0x100000, 0x30000);
            p->icon->s46(0x100000, 0x30000);
            x = (1 - p->pick) * 0x2a;
            p->icon->s13(0, (x + p->h2c) << 12, 0);
            p->icon->s11();
            func_0202f7e8(func_02009900(), p->icon, 1, 0);
        }
    }
    sprite = (Sprite *)func_0202f624(0xc8);
    if (sprite != 0) {
        sprite = func_0205a7a0(sprite);
    }
    p->marker = sprite;
    if (p->marker != 0) {
        p->marker->s42(0xc1, 0, 1, 0, 1, -1, 0);
        p->marker->s33(0x430);
        func_0205765c(p->marker, 0, 0, 0x100000, 0x30000);
        p->marker->s46(0x100000, 0x30000);
        p->marker->s13(0, (p->h2c + 0x2a) << 12, 0);
        p->marker->s11();
        func_0202f7e8(func_02009900(), p->marker, 1, 0);
    }
    p->b13 = 1;
}

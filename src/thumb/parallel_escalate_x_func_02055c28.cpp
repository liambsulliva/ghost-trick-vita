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
    virtual void s33(int value);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(unsigned short id, int a, int b, int c, int d, int e, int f);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int x, int y);
};

struct PanelBits {
    unsigned char mode : 2;
    unsigned char hidden : 1;
    unsigned char closing : 1;
    unsigned char wide : 1;
};

struct PanelInfo {
    unsigned short icon;
    unsigned short height;
    unsigned short pad;
};

class Panel {
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
    unsigned char pad4[0x24];
    struct PanelBits bits;
    unsigned char pad29;
    unsigned short id;
    int y;
    int offset;
    int top;
    unsigned short height;
    unsigned short pad3a;
    int x;
    int x2;
    int speed;
    Sprite *sprites[4];
};

extern "C" {
extern struct PanelInfo data_02099cac[];
extern void *func_0201003c(void);
extern int func_02015a88(void *menu, int id);
extern void func_0205765c(Sprite *sprite, int a, int b, int c, int d);
extern void func_02015954(void *menu);
void func_02055c28(Panel *p, int id, unsigned char wide, int flat);
}

void func_02055c28(Panel *p, int id, unsigned char wide, int flat)
{
    unsigned int i;

    p->bits.mode = 1;
    p->bits.hidden = 0;
    p->bits.closing = 0;
    p->bits.wide = wide;
    p->id = id;
    p->y = 0;
    p->offset = 0;
    p->top = 0x100;
    if (func_02015a88(func_0201003c(), id) != 0) {
        p->top = 0xe8;
    }
    if (flat != 0) {
        p->height = 0;
    } else {
        p->height = data_02099cac[id].height;
    }
    p->y = p->top;
    p->x = -0x9c000;
    p->x2 = 0x44000;
    p->speed = 0;
    p->sprites[0]->s33(0x430);
    for (i = 1; i < 4; i++) {
        p->sprites[i]->s33(0x420);
    }
    if (data_02099cac[p->id].icon != 0xffff) {
        p->sprites[0]->s42(data_02099cac[p->id].icon, 0, 1, 0, 1, -1, 0);
        func_0205765c(p->sprites[0], 0, 0, 0x40000, 0x40000);
        p->sprites[0]->s46(0x40000, 0x40000);
        p->sprites[0]->s10();
    } else {
        p->sprites[0]->s43();
        p->sprites[0]->s11();
    }
    p->sprites[1]->s42(0xc7, 0, 1, 0, 1, -1, 0);
    func_0205765c(p->sprites[1], 0, 0, 0x10000, 0x38000);
    p->sprites[1]->s46(0x10000, 0x38000);
    p->sprites[1]->s10();
    p->sprites[2]->s42(0xc7, 0, 1, 0, 1, -1, 0);
    func_0205765c(p->sprites[2], 0x10000, 0, 0x1f000, 0x38000);
    p->sprites[2]->s46(0xb0000, 0x38000);
    p->sprites[2]->s10();
    p->sprites[3]->s42(0xc7, 0, 1, 0, 1, -1, 0);
    func_0205765c(p->sprites[3], 0xf000, 0, 0, 0x38000);
    p->sprites[3]->s46(0x10000, 0x38000);
    p->sprites[3]->s10();
    if (p->bits.wide) {
        p->offset = 0x40;
    }
    p->s10();
    func_02015954(func_0201003c());
}

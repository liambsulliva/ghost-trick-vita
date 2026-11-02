class Label {
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
};

class Menu {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int a);

    char pad4[0x24];
    unsigned char state : 2;
    unsigned char opened : 1;
    unsigned char done : 1;
    unsigned char rest : 4;
    char pad29[3];
    int x;
    int pos;
    int base;
    unsigned short sound;
    char pad3a[0xe];
    Label *title;
    Label *left;
    Label *middle;
    Label *right;
};

extern "C" {
extern unsigned char data_020cff60[];
extern void *func_0200847c(void);
extern void func_0202a0c0(void *system, int sound);
extern void *func_0201003c(void);
extern void func_02015944(void *object);
extern int func_020151e4(void *object);
extern void func_02015990(void *object, int value);
void func_02055ea8(Menu *menu);
}

void func_02055ea8(Menu *menu)
{
    int speed;

    speed = (data_020cff60[1] + 1) * 8;
    switch (menu->state) {
    case 1:
        menu->pos += speed;
        if (menu->pos > 0x40) {
            menu->pos = 0x40;
            menu->opened = 1;
            func_02015944(func_0201003c());
        }
        menu->title->s13((menu->x - (menu->base + 0x60)) << 12, (0x80 - menu->pos) << 12, 0);
        menu->left->s13((menu->x - 0x148) << 12, (0x84 - menu->pos) << 12, 0);
        menu->middle->s13((menu->x - 0xe8) << 12, (0x84 - menu->pos) << 12, 0);
        menu->right->s13((menu->x - 0x88) << 12, (0x84 - menu->pos) << 12, 0);
        if (menu->opened && !menu->done) {
            menu->done = 1;
            func_0202a0c0(func_0200847c(), menu->sound);
            if (func_020151e4(func_0201003c()) != 0) {
                func_02015990(func_0201003c(), 0);
            }
        }
        break;
    case 2:
        if (menu->pos < 0) {
            menu->state = 0;
            menu->s8(0);
            return;
        }
        menu->pos -= speed;
        menu->title->s13((menu->x - (menu->base + 0x60)) << 12, (0x80 - menu->pos) << 12, 0);
        menu->left->s13((menu->x - 0x148) << 12, (0x84 - menu->pos) << 12, 0);
        menu->middle->s13((menu->x - 0xe8) << 12, (0x84 - menu->pos) << 12, 0);
        menu->right->s13((menu->x - 0x88) << 12, (0x84 - menu->pos) << 12, 0);
        break;
    }
}

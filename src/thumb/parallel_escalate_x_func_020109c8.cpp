class Button {
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
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual int s47(int x, int y);
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual int s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
};

struct Owner {
    char pad0[8];
    unsigned int flags8_low : 16;
    unsigned int mode : 8;
    unsigned int flags8_high : 8;
    char pad0c[0xdf0];
    Button *buttons[8];
    int timers[8];
};

extern "C" {
extern unsigned int data_020c9d50[];
extern int func_02010b38(Owner *owner);
extern int func_0200f330(Owner *owner);
extern int func_02010b54(Button *button);
extern void *func_02010b7c(void);
extern int func_02010b60(void *touch);
extern int func_02010b64(void *touch);
extern int func_020109c0(Button *button);
extern int func_0202f64c(Button **button);
void func_020109c8(Owner *owner, int a, int b);
}

void func_020109c8(Owner *owner, int a, int b)
{
    int pressed;
    unsigned int i;

    if (a == 0 && b == 0 && owner->mode == 3 && func_02010b38(owner) == 0 && func_0200f330(owner) == 0) {
        pressed = 0;
        if (owner->buttons[1] != 0 && func_02010b54(owner->buttons[1]) != 0) {
            if ((data_020c9d50[1] & 0x100000) != 0 || owner->buttons[1]->s47(func_02010b60(func_02010b7c()), func_02010b64(func_02010b7c())) != 0) {
                owner->buttons[1]->s64();
                pressed = 1;
            }
        }
        if (owner->buttons[0] != 0 && func_02010b54(owner->buttons[0]) != 0 && pressed == 0) {
            if ((data_020c9d50[1] & 0x80000) != 0 || owner->buttons[0]->s47(func_02010b60(func_02010b7c()), func_02010b64(func_02010b7c())) != 0) {
                owner->buttons[0]->s64();
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (owner->buttons[i] != 0 && func_02010b54(owner->buttons[i]) != 0 && owner->timers[i] > 0) {
            owner->timers[i] = owner->timers[i] - 1;
        }
        if (owner->buttons[i] != 0) {
            if (func_020109c0(owner->buttons[i]) != 0) {
                func_0202f64c(&owner->buttons[i]);
            } else if (owner->timers[i] == 0 && owner->buttons[i]->s59() == 0) {
                owner->buttons[i]->s60();
            }
        }
    }
}

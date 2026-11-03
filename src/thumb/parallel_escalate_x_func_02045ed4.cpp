class Popup {
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
    virtual void s13(int a, int b, int c);
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22(int a, int b, int c);
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
    virtual void s47();
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
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68(Popup *other, void *data, int flag);
    virtual int s69(Popup *other, void *data);
};

struct PopupData {
    char pad00[0xe0];
    unsigned char e0_low : 2;
    unsigned char closing : 1;
    unsigned char e0_high : 5;
    char pade1[0x4f];
    unsigned char low : 3;
    unsigned char phase : 2;
    unsigned char mode : 3;
    char pad131[3];
    int scale;
    int scale_target;
    int scale_speed;
    int t;
    int t_step;
    int from_x;
    int from_y;
    int to_x;
    int to_y;
    int f158;
    int f15c;
};

extern "C" {
extern char data_0209b594[];
extern void func_02045a78(Popup *popup, int a, int b, int c, int d, int e, int f, int g);
extern int func_020109c0(Popup *popup);
extern void *func_02008444(void);
extern int func_0200f330(void *game);
extern void func_020113f8(void *game);
extern void func_02010b74(Popup *popup);
extern int func_0206f28c(int a, int b);
void func_02045ed4(Popup *popup);
}

void func_02045ed4(Popup *popup)
{
    struct PopupData *self;
    int found;
    int dy;
    int x;

    self = (struct PopupData *)popup;
    if (self->phase == 1) {
        self->phase = 3;
    } else if (self->phase == 2) {
        func_02045a78(popup, 2, 0xf0, 0, 1, self->from_x, self->from_y, 7);
        self->phase = 0;
    } else if (self->phase == 3) {
        func_02045a78(popup, 1, 0x420, self->f15c, 0, 0, 0, 8);
        self->phase = 0;
    }
    if (func_020109c0(popup) != 0) {
        return;
    }
    if ((self->mode != 3 || self->closing != 0) && func_0200f330(func_02008444()) != 0) {
        return;
    }
    found = popup->s69(popup, data_0209b594);
    if (self->mode == 2) {
        return;
    }
    if (self->mode == 3) {
        if (self->closing == 0) {
            func_020113f8(func_02008444());
        }
        if (found == 0) {
            if (self->closing != 0) {
                func_02010b74(popup);
            } else {
                popup->s65();
            }
        }
        return;
    }
    popup->s22(self->scale, self->scale, 0x1000);
    self->scale += self->scale_speed;
    if (self->scale > self->scale_target + 0x333) {
        self->scale = self->scale_target + 0x333;
        self->scale_speed = -0x29;
    }
    if (self->scale < self->scale_target) {
        self->scale = self->scale_target;
        self->scale_speed = 0x29;
    }
    self->scale_target += 0x52;
    if (self->scale_target > 0xb33) {
        self->scale_target = 0xb33;
    }
    self->t += self->t_step;
    if (self->t > 0x1000) {
        self->mode = 2;
        self->t = 0x1000;
        popup->s68(popup, data_0209b594, 0);
        popup->s13(0, 0x4e000, 0);
        popup->s22(0x1000, 0x1000, 0x1000);
        return;
    }
    dy = self->to_y - self->from_y;
    x = self->from_x + func_0206f28c(self->to_x - self->from_x, self->t);
    popup->s13(x, self->from_y + func_0206f28c(dy, self->t), 0);
}

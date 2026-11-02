class Arm {
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
};

typedef int code(...);

struct Vec {
    int x;
    int y;
    int z;
};

struct Table {
    int v[12];
};

struct ArmData {
    char pad00[0x34];
    int scale_x;
    int scale_y;
    char pad3c[0x7c];
    int *model;
    int fbc;
    char padc0[4];
    unsigned short state;
    unsigned short shown;
    int reach;
    int fcc;
    int fd0;
    int fd4;
};

extern "C" {
extern struct Table data_0209bafc;
extern void func_020471ec(struct Vec *out, Arm *arm);
extern void func_02047250(struct Vec *out, Arm *arm);
extern void func_020083c0(struct Vec *dst, struct Vec *a, struct Vec *b);
extern int func_020452e8(struct Vec *v);
extern int func_0206ef28(int a, int b);
extern unsigned short func_0206f2a4(int value);
extern int func_0204cdc4(unsigned short angle);
extern int func_0204cdd8(unsigned short angle);
extern void func_020082d8(struct Vec *dst, struct Vec *src);
extern int func_0206f28c(int a, int b);
extern void *func_02008444(void);
extern int func_0200f330(void *game);
void func_0204cbd0(Arm *arm);
}

void func_0204cbd0(Arm *arm)
{
    struct ArmData *self;
    struct Table table;
    struct Vec pos;
    struct Vec target;
    struct Vec diff;
    struct Vec tip;
    struct Vec out;
    int len;
    unsigned short angle;
    unsigned short side;
    int c;
    int s;
    int sx;
    int sy;

    self = (struct ArmData *)arm;
    if (self->state != self->shown) {
        table = data_0209bafc;
        if (self->state == 4) {
            self->reach -= 0x340;
            if (self->reach < 0xc000) {
                self->reach = 0xc000;
                self->shown = self->state;
            }
            self->fcc = table.v[11];
            self->fd0 = table.v[10];
            self->fd4 = table.v[9];
        } else {
            self->fcc = table.v[self->state * 3 + 2];
            self->fd0 = table.v[self->state * 3 + 1];
            self->fd4 = table.v[self->state * 3];
            self->shown = self->state;
        }
    }
    func_020471ec(&pos, arm);
    func_02047250(&target, arm);
    func_020083c0(&diff, &target, &pos);
    len = func_020452e8(&diff);
    angle = 0;
    if (len > 0) {
        angle = func_0206f2a4(func_0206ef28(diff.x, len));
        if (diff.y > 0) {
            angle = -angle;
        }
        angle -= 0xfb7;
    }
    c = func_0204cdc4(angle);
    s = func_0204cdd8(angle);
    func_020082d8(&tip, &pos);
    tip.x = pos.x + func_0206f28c(self->fcc, c);
    tip.y = pos.y + func_0206f28c(-self->fcc, s);
    side = angle + 0x4000;
    c = func_0204cdc4(side);
    s = func_0204cdd8(side);
    tip.x += func_0206f28c(0xa000, c);
    tip.y += func_0206f28c(-0xa000, s);
    func_020082d8(&out, &tip);
    (**(code **)(*self->model + 0xbc))(self->model, &out, self->fbc, self->reach);
    sx = func_0206f28c(self->fd0, self->scale_x);
    sy = func_0206f28c(self->fd4, self->scale_y);
    (**(code **)(*self->model + 0x58))(self->model, sx, sy, 0x1000);
    (**(code **)(*self->model + 0x78))(self->model, angle);
    if (func_0200f330(func_02008444()) == 0) {
        arm->s65();
    }
}

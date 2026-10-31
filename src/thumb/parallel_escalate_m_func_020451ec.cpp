struct Vec {
    int x;
    int y;
    int z;
};

class Actor {
public:
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39();
    virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43();
    virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51();
    virtual void s52(); virtual void s53(); virtual void s54(); virtual void s55();
    virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59();
    virtual void s60(); virtual void s61();
    virtual void update();

    char pad4[8];
    unsigned char kind;
    char pad0d[0x1b];
    int x;
    int y;
    char pad30[0x97];
    unsigned char busy;
    char padc8[0x50];
    signed char state;
    unsigned char locked;
    unsigned char near;
    char pad11b[9];
    int range;
};

extern "C" {
extern unsigned int *func_02008444(void);
extern int func_0202c394(unsigned int *system);
extern int func_0203435c(unsigned int *system);
extern void *func_02010194(unsigned int *system);
extern void func_0200f70c(Vec *vec);
extern int func_0200f5c8(unsigned int *system);
extern int func_0200f5e8(unsigned int *system);
extern void func_02058a80(Vec *dst, void *source);
extern void func_020083c0(Vec *dst, Vec *a, Vec *b);
extern int func_020452e8(Vec *vec);
extern void func_0200ee34(Actor *actor, int r, int g, int b);
extern unsigned char func_02010b54(Actor *actor);
extern void func_020570cc(Actor *actor);
}

extern "C" void func_020451ec(Actor *self)
{
    Vec offset;
    Vec diff;
    Vec base;
    void *target;
    unsigned char isNear;

    if (self->state == -1) {
        return;
    }
    if (self->state == 1) {
        self->state = 0;
        return;
    }
    if (self->busy != 0) {
        return;
    }
    if (func_0202c394(func_02008444()) != 0) {
        return;
    }
    self->update();
    if (self->locked == 0) {
        target = func_02010194(func_02008444());
        if (target != 0) {
            func_0200f70c(&offset);
            offset.x = self->x + func_0200f5c8(func_02008444()) + 0x80000;
            offset.y = -self->y + func_0200f5e8(func_02008444()) + 0x60000;
            func_02058a80(&base, target);
            func_020083c0(&diff, &base, &offset);
            isNear = func_020452e8(&diff) >= self->range;
            if (isNear != self->near) {
                self->near = isNear;
                if (self->near != 0) {
                    func_0200ee34(self, 0x10, 0x10, 0x10);
                } else {
                    func_0200ee34(self, 0x1f, 0x1f, 0x1f);
                }
            }
        }
    }
    if (self->kind == 3 && func_02010b54(self) && func_0203435c(func_02008444()) == 0) {
        func_020570cc(self);
    }
}

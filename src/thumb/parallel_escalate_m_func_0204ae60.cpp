struct Vec {
    int x;
    int y;
    int z;
};

class Sprite {
public:
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11();
    virtual void setPos(Vec *pos);
    virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void update();
    virtual void s33(); virtual void s34();
    virtual void setA(signed char value);
    virtual void s36();
    virtual void setB(signed char value);
};

struct Actor {
    char pad0[0xb8];
    Sprite *sprite;
    Vec *points;
    char pad_c0[4];
    unsigned short color;
};

extern "C" {
extern unsigned int *func_02008444(void);
extern int func_0202c394(unsigned int *system);
extern int func_0203435c(unsigned int *system);
extern unsigned char func_02010b54(Actor *actor);
extern void func_0200ee34(Sprite *sprite, int color, int a, int b);
extern void func_020082d8(Vec *dst, Vec *src);
extern int func_0200f5c8(unsigned int *system);
extern int func_0200f5e8(unsigned int *system);
}

extern "C" void func_0204ae60(Actor *actor)
{
    unsigned int i;
    Vec pos;

    if (func_0202c394(func_02008444()) != 0) {
        return;
    }
    if (func_0203435c(func_02008444()) != 0) {
        return;
    }
    if (!func_02010b54(actor)) {
        return;
    }
    func_0200ee34(actor->sprite, actor->color, 0, 0);
    for (i = 0; i < 0x10; i++) {
        func_020082d8(&pos, &actor->points[(i + 1) * 2 - 1]);
        pos.x = pos.x - func_0200f5c8(func_02008444()) - 0x80000;
        pos.y = -pos.y + func_0200f5e8(func_02008444()) + 0x60000;
        if (pos.x <= 0x84000 && pos.x >= -0x84000 && pos.y <= 0x64000 && pos.y >= -0x64000) {
            actor->sprite->setPos(&pos);
            actor->sprite->setA((signed char)(i * 2 + 1));
            actor->sprite->setB((signed char)(i + 1));
            actor->sprite->update();
        }
    }
}

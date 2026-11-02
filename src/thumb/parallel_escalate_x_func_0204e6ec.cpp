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
    virtual void s22(int x, int y, int z);
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30(unsigned short a);
    virtual int s31();
    virtual void s32();
    virtual void s33(int value);
    virtual void s34();
    virtual void s35(signed char a);
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int id, int a, int b, int c, int d, int e, int f);
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46(int a, int b);
};

struct Range {
    short h0;
    short h2;
};

class Swarm : public Sprite {
public:
    unsigned char pad04[0xc];
    unsigned char state;
    unsigned char pad11[0xb5];
    short timer[0x20];
    unsigned char pad106[0x20];
    unsigned char kind[0x20];
    unsigned char pad146[2];
    Sprite *glow;
    unsigned char cooldown;
    unsigned char pad14d[3];
    int y;
    int vy;
    int ymin;
    int ymax;
};

extern "C" {
extern char data_020cf724[];
extern struct Range data_0209bda0[];
extern Sprite *func_0205693c(char *name);
extern void func_020153d8(Sprite *sprite, Swarm *owner);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, Sprite *sprite, int a, int b);
extern void func_0200ee34(Swarm *p, int r, int g, int b);
extern void func_0204eb98(Swarm *p, unsigned int i);
extern int func_02008eb0(void);
extern unsigned int func_01ff80b8(int seed);
void func_0204e6ec(Swarm *p);
}

void func_0204e6ec(Swarm *p)
{
    unsigned int i;

    if (p->state == 0) {
        p->state = 1;
        return;
    }
    if (p->state == 1) {
        p->glow = func_0205693c(data_020cf724);
        p->glow->s42(0xb0, 0, 1, 0, 1, -1, 0);
        p->glow->s46(0x80000, 0x80000);
        p->glow->s22(0x2900, 0x2900, 0x1000);
        p->glow->s33(0x724);
        func_020153d8(p->glow, p);
        func_0202f7e8(func_02009900(), p->glow, 7, 0);
        p->ymin = 0;
        p->ymax = 0x14;
        p->y = 0x14000;
        p->vy = 0;
        p->s42(0xaf, 0, 1, 0, 1, -1, 0);
        p->s46(0x20000, 0x20000);
        p->s33(0x714);
        func_0200ee34(p, 0x18, 0x18, 0x18);
        for (i = 0; i < 0x20; i++) {
            p->timer[i] = -1;
            if (i < 0x14) {
                func_0204eb98(p, i);
                p->timer[i] = func_01ff80b8(func_02008eb0()) % data_0209bda0[p->kind[i]].h2;
            }
        }
        p->state = 2;
        return;
    }
    p->glow->s35(p->y >> 12);
    p->y += p->vy;
    if (p->y < p->ymin << 12) {
        p->y = p->ymin << 12;
        p->vy *= -1;
    }
    if (p->y > p->ymax << 12) {
        p->y = p->ymax << 12;
        p->vy *= -1;
    }
    p->glow->s30(p->glow->s31() + 0x20);
    for (i = 0; i < 0x20; i++) {
        p->timer[i]++;
        if (p->timer[i] > data_0209bda0[p->kind[i]].h2) {
            p->timer[i] = -1;
        } else if (p->cooldown == 0 && p->timer[i] < 0) {
            func_0204eb98(p, i);
            p->cooldown = 1;
        }
    }
    p->cooldown++;
    if (p->cooldown > 0x14) {
        p->cooldown = 0;
    }
}

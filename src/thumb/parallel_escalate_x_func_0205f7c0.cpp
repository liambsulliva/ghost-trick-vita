struct Pad {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int held;
};

struct WaveFlags {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
};

class Wave {
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
    unsigned char pad4[0x40];
    unsigned char fade;
    unsigned char pad45[0x37];
    struct WaveFlags flags;
    unsigned char pad7d[2];
    unsigned char count;
    unsigned char age;
    unsigned char hold;
    unsigned char pad82[2];
    unsigned short tick;
    short fade_in;
    short fade_out;
    short fade_time;
    unsigned char pad8c[0x0c];
    unsigned char head[0x38];
    unsigned char *lines;
    unsigned char padd4[8];
    void *graph;
    unsigned char pade0[0x0c];
    unsigned short busy;
    unsigned char padee[0x12];
    int target;
    unsigned short limit;
};

extern "C" {
extern struct Pad data_020c9d50;
extern void *func_02008444(void);
extern int func_0203435c(void *system);
extern int func_0205d0c0(void *system);
extern int func_0205fa18(void *system);
extern int func_020328e4(void *system);
extern int func_0205c230(void *system);
extern void func_02060d5c(Wave *p);
extern int func_0205f054(void *graph);
extern void func_0205eaec(void *graph, int on);
extern void func_0205f074(void *graph, int color);
extern void func_0205eb18(void *graph);
extern void func_020612b8(Wave *p);
extern void func_02061388(Wave *p);
extern void func_02060d54(Wave *p, int state);
extern void func_02060f54(Wave *p, int value);
extern void *func_02010b7c(void);
extern int func_02015154(void *object);
extern void func_0205fa2c(Wave *p, int a, int b);
extern void func_020606f0(Wave *p);
extern void func_020083c0(void *dst, void *a, void *b);
extern int func_020452e8(void *v);
extern void func_0206001c(Wave *p, int a, int b);
extern void func_020608d8(Wave *p);
extern void func_02060a14(Wave *p);
void func_0205f7c0(Wave *p);
}

void func_0205f7c0(Wave *p)
{
    unsigned char diff[12];
    int value;
    int held;

    if (func_0203435c(func_02008444()) != 0) {
        return;
    }
    if (func_0205d0c0(func_02008444()) != 0) {
        p->s33(0x3a0);
    } else {
        p->s33(0x8e0);
    }
    func_02060d5c(p);
    p->flags.b7 = 0;
    if (p->busy == 0) {
        return;
    }
    if (p->fade_in > 0) {
        p->fade = p->fade_time * 0x1f / p->fade_in;
        p->fade_time = p->fade_time + 1;
        if (p->fade_time > p->fade_in) {
            p->fade_in = 0;
        }
    }
    if (p->fade_out > 0) {
        p->fade = 0x1f - 0x1f * p->fade_time / p->fade_out;
        p->fade_time = p->fade_time + 1;
        if (p->fade_time > p->fade_out) {
            p->fade_out = 0;
        }
    }
    if (p->graph != 0) {
        if (func_0205fa18(func_02008444()) != 0) {
            if (func_0205f054(p->graph) != 0) {
                func_0205eaec(p->graph, 0);
            }
        } else {
            if (func_0205f054(p->graph) == 0) {
                func_0205eaec(p->graph, 1);
            }
        }
        if (func_020328e4(func_02008444()) != 0 && func_0205c230(func_02008444()) == 0) {
            func_0205f074(p->graph, 0x3500);
        } else {
            func_0205f074(p->graph, 0x76c0);
        }
    }
    func_020612b8(p);
    func_02061388(p);
    if (p->flags.b3) {
        p->flags.b3 = 0;
        func_0205f074(p->graph, 0x76c0);
        p->limit = 0x7fff;
        p->target = 0;
        p->tick = 0;
        p->flags.b0 = 0;
        p->hold = 0xf;
        func_02060d54(p, 0);
        func_02060f54(p, 1);
        func_0205eb18(p->graph);
    }
    value = func_02015154(func_02010b7c());
    held = 0;
    if (data_020c9d50.cur & 0xf00) {
        held = 1;
    }
    p->age = p->age + 1;
    if (p->age > 0x1e) {
        p->age = 0x1e;
    }
    func_0205fa2c(p, value, held);
    func_020606f0(p);
    func_020083c0(diff, p->head, p->lines + (p->count - 1) * 12);
    if (func_020452e8(diff) < 0x800) {
        p->flags.b1 = 1;
    }
    func_0206001c(p, value, held);
    func_020608d8(p);
    func_02060a14(p);
}

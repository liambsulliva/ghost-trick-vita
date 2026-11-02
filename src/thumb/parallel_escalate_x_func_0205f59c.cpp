class Res {
public:
    virtual void s0();
    virtual void s1();
    virtual void *s2(int size, void *name);
};

struct Pair {
    void *low;
    void *high;
};

struct WaveIds {
    int ids[8];
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
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37(int value);
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int id, int a, int b, int c, int d, int e, int f);
    unsigned char pad4[0x7b];
    unsigned char count;
    unsigned char pad80[0x2c];
    int step;
    unsigned char padb0[0x20];
    void *lines;
    void *marks;
    void *spans;
    void *graph;
    struct Pair *pairs;
    void *heights;
    void *slots;
    unsigned char padec[0x18];
    unsigned short limit;
};

extern "C" {
extern char data_0209dc18[];
extern const struct WaveIds data_0209dbb8;
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *system);
extern void func_0204dc90(Wave *p, int value);
extern void func_0205f7b8(void *lines, int a, int b, int c);
extern void *func_0205f55c(int id, int high);
extern void *func_02031254(int size);
extern void *func_0205e8bc(void *object);
extern void func_0205e930(void *graph, int count, int a, int b, int c, int d, int e, int f);
extern void func_0205ea04(void *graph, int a, int b, int c, int d, int e, int f, int g, int h, int i);
void func_0205f59c(Wave *p);
}

void func_0205f59c(Wave *p)
{
    struct WaveIds table;
    unsigned char i;
    void *object;

    p->s42(0x1c, 0, 1, 0, 1, -1, 0);
    p->s37(0x3b);
    func_0204dc90(p, 0x800);
    p->s33(0x8e0);
    p->count = 8;
    p->step = 0x1000 / (p->count - 1);
    p->lines = func_02007b70(func_02007bd8())->s2(p->count * 12, data_0209dc18);
    p->marks = func_02007b70(func_02007bd8())->s2(p->count * 12, data_0209dc18);
    p->spans = func_02007b70(func_02007bd8())->s2(p->count * 12, data_0209dc18);
    p->pairs = (struct Pair *)func_02007b70(func_02007bd8())->s2(p->count * 8, data_0209dc18);
    p->heights = func_02007b70(func_02007bd8())->s2(p->count * 12, data_0209dc18);
    p->slots = func_02007b70(func_02007bd8())->s2(p->count * 0x24, data_0209dc18);
    func_0205f7b8(p->lines, 0, 0, 0);
    table = data_0209dbb8;
    for (i = 0; i < p->count; i++) {
        p->pairs[i].low = func_0205f55c(table.ids[i], 0);
    }
    for (i = 0; i < p->count; i++) {
        p->pairs[i].high = func_0205f55c(i * 0x52 + 0x99a, 1);
    }
    p->limit = 0x7fff;
    object = func_02031254(0x78c);
    if (object != 0) {
        object = func_0205e8bc(object);
    }
    p->graph = object;
    func_0205e930(p->graph, p->count, -1, 0x2666, 0, 0x910, 0x18, 0x59a);
    func_0205ea04(p->graph, 7, 4, 7, 6, 7, 4, 0, 7, 5);
}

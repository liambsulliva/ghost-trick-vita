class Item {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
};

struct Rack {
    char pad00[0x34];
    int speed;
    char pad38[0x40];
    Item *front;
    Item *back;
    char pad80[8];
    Item *upper[2];
    Item *lower[2];
    unsigned char slots[4];
};

extern "C" {
extern void func_02065398(Rack *rack);
extern void func_02065178(Rack *rack);
extern void *func_02031254(int size);
extern Item *func_02065048(void *p, unsigned char id);
extern Item *func_02065064(void *p, unsigned char id);
extern int func_02065328(Rack *rack, Item *item, unsigned char *a, unsigned char *b);
extern int func_0207b424(int value);
extern int func_0207bd24(int a, int b);
extern int func_0207bf54(int a, int b);
extern long long func_0207b364(int value);
extern void func_02065524(Item *item, long long value);
extern void func_020652d8(Rack *rack);
void func_02064ee0(Rack *rack);
}

void func_02064ee0(Rack *rack)
{
    unsigned int i;
    void *p;
    int v;
    int r;

    if (rack->front != 0) {
        rack->front->s2();
    }
    if (rack->back != 0) {
        rack->back->s2();
    }
    func_02065398(rack);
    func_02065178(rack);
    for (i = 0; i < 2; i++) {
        p = func_02031254(0x24);
        if (p != 0) {
            p = func_02065048(p, (unsigned char)(i * 2 + 1));
        }
        rack->upper[i] = (Item *)p;
        if (func_02065328(rack, rack->upper[i], &rack->slots[0], &rack->slots[1]) != 0) {
            rack->upper[i]->s0();
            rack->upper[i]->s1();
            v = -(int)(i * 0x100 + 0xc0);
            if (v != 0) {
                r = func_0207bd24(0x3f000000, func_0207b424(v << 12));
            } else {
                r = func_0207bf54(func_0207b424(v << 12), 0x3f000000);
            }
            func_02065524(rack->upper[i], func_0207b364(r));
        }
        p = func_02031254(0x24);
        if (p != 0) {
            p = func_02065064(p, (unsigned char)(i * 2));
        }
        rack->lower[i] = (Item *)p;
        if (func_02065328(rack, rack->lower[i], &rack->slots[2], &rack->slots[3]) != 0) {
            rack->lower[i]->s0();
            rack->lower[i]->s1();
            v = -(int)(i * 0x100 + 0x180);
            if (v != 0) {
                r = func_0207bd24(0x3f000000, func_0207b424(v << 12));
            } else {
                r = func_0207bf54(func_0207b424(v << 12), 0x3f000000);
            }
            func_02065524(rack->lower[i], func_0207b364(r));
        }
    }
    func_020652d8(rack);
    rack->speed = 0x800;
}

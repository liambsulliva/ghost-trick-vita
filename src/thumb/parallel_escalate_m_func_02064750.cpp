class Obj064 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15(int);
    virtual void slot16();
    virtual void slot17();
    virtual void slot18(int);
    virtual void slot19(int, int);
    virtual void slot20();
    virtual void slot21(int, int);
    virtual void slot22();
    char pad04[0xc];
    unsigned char state;
    char pad11[0x17];
    int idx;
    int timer;
    int f30;
    int f34;
    int f38;
    int f3c;
    char pad40[0x14];
    unsigned int pos;
    int f58;
    unsigned int limit;
    int f60;
    int f64;
    int a68;
    int b6c;
    int c70;
    int c74;
    char f78;
    unsigned char f79;
    unsigned char f7a;
};

extern "C" {
extern int func_02064854(int value);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *object, int a, int b, int c, int d);

void func_02064750(Obj064 *p)
{
    if (p->timer != 0) {
        p->timer = p->timer - 1;
        return;
    }
    switch (p->state) {
    case 1:
        p->state = 2;
        if (p->pos < p->limit) {
            return;
        }
        p->state = 3;
        return;
    case 2:
        p->slot22();
        if (func_02064854(p->a68) >= 0x30) {
            int step = 0x1800;
            p->slot19(p->c70 * step, 0);
            p->c70 = p->c70 + 1;
            p->slot21(p->a68 - step * 0x20, 0);
        }
        if (func_02064854(p->b6c) >= 0x30) {
            int step = 0x1800;
            p->slot19(p->c74 * step, 1);
            p->c74 = p->c74 + 1;
            p->slot21(p->b6c - step * 0x20, 1);
        }
        if (p->f7a == 0) {
            if (p->pos == p->f79 * 0xc0) {
                p->state = 1;
                p->timer = 0x78;
                return;
            }
        } else {
            int one = 1;
            if (p->pos >= 0xc0 && p->c74 == 0) {
                p->slot19(p->pos - 0xc0, one);
                func_0202f1bc(func_02009c1c(), p->f3c, 0, one << 11, 0x1b);
                p->c74 = p->c74 + 1;
            }
            if (p->pos >= p->limit) {
                p->state = 3;
            }
        }
        break;
    case 3:
        break;
    }
}
}

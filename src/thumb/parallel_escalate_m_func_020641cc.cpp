struct Vec { int x; int y; int z; };

class Kid0641 {
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
    virtual void slot13(int, int, int);
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46(int, int);
};

class Obj0641 {
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
    virtual void slot12(Vec *v);
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual Vec *slot17();
    char pad04[0xc4];
    Kid0641 *kids[4];
};

extern "C" {
extern void *func_02008eb0(void);
extern unsigned int func_01ff80b8(void *object);
extern void func_0205765c(Obj0641 *object, int a, int b, int c, int d);
extern void func_020082d8(Vec *dst, Vec *src);

void func_020641cc(Obj0641 *p)
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int x;
    unsigned int rem;
    unsigned int z;
    unsigned int i;
    int one;
    Vec v;

    a = func_01ff80b8(func_02008eb0());
    b = func_01ff80b8(func_02008eb0());
    c = func_01ff80b8(func_02008eb0()) & 0x1f;
    d = func_01ff80b8(func_02008eb0());
    func_0205765c(p, (a & 0x1f) << 12, (b & 0xf) << 12, (c + 0x20) << 12, ((d & 0xf) + 0x10) << 12);
    func_020082d8(&v, p->slot17());
    v.y = v.y + 0x2000;
    if (v.y > 0x90000) {
        v.y = -0x90000;
    }
    p->slot12(&v);
    one = 1;
    i = 0;
    do {
        x = func_01ff80b8(func_02008eb0()) & 0x7f;
        rem = func_01ff80b8(func_02008eb0()) % 0x60;
        z = func_01ff80b8(func_02008eb0()) & 0x1f;
        p->kids[i]->slot13((x * ((i & ~one) - 1)) << 12, (rem * ((i & 1) * 2 - 1)) << 12, 0);
        p->kids[i]->slot46((z + 0x1f) << 12, 0x2000);
        i = i + 1;
    } while (i < 4);
}
}

class Badge {
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
    unsigned char pad4[0x24];
    int x;
    int y;
    unsigned char pad30[0x12];
    unsigned short base_y;
    signed char color;
    signed char alpha;
    unsigned char pad46[0x1e];
    int poly_id;
    unsigned char pad68[0x50];
    void *image;
};

extern "C" {
extern void *func_0202970c(void);
extern int func_02029708(void *object);
extern void func_02048734(void);
extern void func_02048740(int first, int second, int third);
extern void func_02048750(int format, int gen, int s, int t, int repeat, int flip, int color0, unsigned int address);
extern void func_0204878c(int a, int b, int c, int d, int e, int f);
extern void func_020487b0(int value);
extern void func_020487bc(int value);
extern void func_020487c8(short x, short y, short z);
extern void func_02048d18(int s, int t);
extern void func_020487e4(void);
extern int func_0202c36c(void *image);
extern int func_0202c370(void *image);
extern int func_0202c374(void *image);
extern unsigned int func_02029020(void *image);
extern void *func_02029024(void *image);
extern void func_020493b0(void *palette, int format);
void func_020491c0(Badge *p);
}

void func_020491c0(Badge *p)
{
    int depth;

    depth = -(func_02029708(func_0202970c()) - p->base_y);
    func_02048734();
    func_02048740(p->x >> 7, (p->y / 3) >> 5, depth);
    p->slot41();

    func_020487b0(1);
    func_020487bc(0x7fff);
    func_02048d18(0, 0);
    func_020487c8(-0x100, 0xc00, 0);
    func_02048d18(0, 0x10000);
    func_020487c8(-0x100, -0xc00, 0);
    func_02048d18(0x10000, 0x10000);
    func_020487c8(0x100, -0xc00, 0);
    func_02048d18(0x10000, 0);
    func_020487c8(0x100, 0xc00, 0);
    func_020487e4();

    func_02048750(func_0202c374(p->image), 1, func_0202c36c(p->image), func_0202c370(p->image), 3, 0, 1, func_02029020(p->image));
    func_020493b0(func_02029024(p->image), func_0202c374(p->image));
    func_0204878c(0, 0, 3, p->alpha, p->color, p->poly_id);

    func_020487b0(1);
    func_020487bc(0x7fff);
    func_02048d18(0, 0);
    func_020487c8(-0x200, 0xe00, 0);
    func_02048d18(0, 0x20000);
    func_020487c8(-0x200, 0x8aa, 0);
    func_02048d18(0x20000, 0x20000);
    func_020487c8(0x200, 0x8aa, 0);
    func_02048d18(0x20000, 0);
    func_020487c8(0x200, 0xe00, 0);
    func_02048d18(0, 0);
    func_020487c8(-0x200, -0xe00, 0);
    func_02048d18(0, 0x1f000);
    func_020487c8(-0x200, -0x8aa, 0);
    func_02048d18(0x20000, 0x1f000);
    func_020487c8(0x200, -0x8aa, 0);
    func_02048d18(0x20000, 0);
    func_020487c8(0x200, -0xe00, 0);
    func_020487e4();
}

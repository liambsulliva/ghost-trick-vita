class VTarget {
public:
    virtual void slot0();
    virtual void slot1();
};

struct Owner {
    int x0;
    int x4;
    int x8;
    VTarget *xc;
    VTarget *x10;
    int x14;
    int x18;
    unsigned int x1c;
    unsigned int x20;
    short x24;
    unsigned short x26;
};

extern "C" {
extern char data_02085274;
extern char data_02085280;
void func_0202a210(Owner *object);
unsigned int func_02007bd8(void);
void func_0201f29c(unsigned int a, VTarget *b);
void func_0202a3a4(Owner *object, unsigned int a);
VTarget *func_02031254(int a);
VTarget *func_0200c504(VTarget *a);
void func_0201e144(VTarget *a, unsigned int b, unsigned int c, int d);
void func_0200c530(VTarget *a, char *b);
void func_0201f288(unsigned int a, VTarget *b);
int func_0201e4d8(int a);
unsigned int func_02029d38(Owner *object, unsigned int a);
void func_0202a268(Owner *object);
}

extern "C" void func_0202a2c0(Owner *p, int mode)
{
    unsigned int value;
    VTarget *obj;
    unsigned int result;
    int size;

    func_0202a210(p);
    func_0201f29c(func_02007bd8(), p->xc);
    func_0201f29c(func_02007bd8(), p->x10);
    if (p->xc != 0) {
        p->xc->slot1();
    }
    if (p->x10 != 0) {
        p->x10->slot1();
    }
    func_0202a3a4(p, p->x20);
    func_0202a3a4(p, p->x1c);
    obj = func_02031254(0x3c);
    if (obj != 0) {
        obj = func_0200c504(obj);
    }
    p->xc = obj;
    obj = func_02031254(0x3c);
    if (obj != 0) {
        obj = func_0200c504(obj);
    }
    p->x10 = obj;
    value = 0x000cffec;
    if (mode != 1) {
        value = 0x00070fec;
    }
    p->x26 = mode;
    result = func_02029d38(p, value);
    p->x1c = result;
    func_0201e144(p->xc, result, value, 0x20);
    func_0200c530(p->xc, &data_02085274);
    func_0201f288(func_02007bd8(), p->xc);
    size = func_0201e4d8(p->x4) - 0x14;
    result = func_02029d38(p, size);
    p->x20 = result;
    func_0201e144(p->x10, result, size, 0x20);
    func_0200c530(p->x10, &data_02085280);
    func_0201f288(func_02007bd8(), p->x10);
    func_0202a268(p);
}

class Heap02019dc4 {
public:
    virtual void h0();
    virtual void h1();
    virtual void h2();
    virtual void h3();
    virtual void h4(void *p);
};

class Obj02016c4c {
public:
    virtual ~Obj02016c4c();
    unsigned char pad4[0x64];
};

struct Slot02019dc4 {
    void *data;
    int size;
};

class Obj02019dc4 : public Obj02016c4c {
public:
    virtual void f0201a04c();
    virtual ~Obj02019dc4();
    void *work;
    void *a[2];
    void *b[2];
    void *c[2];
    void *d[2];
    unsigned char pad8c[0x54];
    Slot02019dc4 slots[3];
};

extern "C" {
extern void *func_02007bd8(void);
extern Heap02019dc4 *func_02007b70(void *system);
}

Obj02019dc4::~Obj02019dc4()
{
    unsigned char i;

    if (work != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            func_02007b70(func_02007bd8())->h4(work);
            work = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (slots[i].data != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->h4(slots[i].data);
                slots[i].data = 0;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (a[i] != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->h4(a[i]);
                a[i] = 0;
            }
        }
        if (b[i] != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->h4(b[i]);
                b[i] = 0;
            }
        }
        if (c[i] != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->h4(c[i]);
                c[i] = 0;
            }
        }
        if (d[i] != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->h4(d[i]);
                d[i] = 0;
            }
        }
    }
}

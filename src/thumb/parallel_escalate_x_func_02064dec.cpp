class Item02064cfc {
public:
    virtual ~Item02064cfc();
};

class Child02064de8 {
public:
    ~Child02064de8();
};

class Heap02064cfc {
public:
    virtual void h0();
    virtual void h1();
    virtual void h2();
    virtual void h3();
    virtual void h4(void *p);
};

class Base020644b0 {
public:
    virtual void b0();
    virtual ~Base020644b0();
    static void operator delete(void *p);
    unsigned char pad4[0x34];
};

class Obj02064cfc : public Base020644b0 {
public:
    virtual void b0();
    virtual ~Obj02064cfc();
    void *bufs[4];
    void *maps[4];
    unsigned char pad58[0x20];
    Item02064cfc *first;
    Item02064cfc *second;
    unsigned char pad80[8];
    Child02064de8 *left[2];
    Child02064de8 *right[2];
};

extern "C" {
extern void *func_02007bd8(void);
extern Heap02064cfc *func_02007b70(void *system);
extern void func_02065398(Obj02064cfc *p);
}

Obj02064cfc::~Obj02064cfc()
{
    unsigned int i;

    if (first != 0) {
        delete first;
        first = 0;
    }
    if (second != 0) {
        delete second;
        second = 0;
    }
    for (i = 0; i < 2; i++) {
        if (*(Child02064de8 **)((char *)this + 0x88 + i * 4) != 0) {
            delete *(Child02064de8 **)((char *)this + 0x88 + i * 4);
            *(Child02064de8 **)((char *)this + 0x88 + i * 4) = 0;
        }
        if (*(Child02064de8 **)((char *)this + 0x90 + i * 4) != 0) {
            delete *(Child02064de8 **)((char *)this + 0x90 + i * 4);
            *(Child02064de8 **)((char *)this + 0x90 + i * 4) = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        if (bufs[i] != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->h4(bufs[i]);
                bufs[i] = 0;
            }
        }
        if (maps[i] != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->h4(maps[i]);
                maps[i] = 0;
            }
        }
    }
    func_02065398(this);
}

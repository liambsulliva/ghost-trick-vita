extern "C" {
extern void func_02029a04(void *object);
extern void func_02031270(void *object);
extern void *func_02042b20(void *object);
extern void func_02042774(void);
extern void func_0207dea4(void *array, int size, int count, void (*dtor)(void));
extern void *func_02007bd8(void);
}

class Manager02028ba8 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(void *handle);
};

extern "C" {
extern Manager02028ba8 *func_02007b70(void *object);
}

class Part02028ba8 {
public:
    ~Part02028ba8() { func_02029a04(this); }
    static void operator delete(void *object) { func_02031270(object); }
};

struct Elem02028ba8 {
    char data[0x54];
};

struct Owner02028ba8 {
    unsigned int count;
    int pad4;
    Elem02028ba8 *elems;
    Part02028ba8 *part;
    void *handle;
    char pad14[0x25];
    unsigned char dirty;
    unsigned char active;
    char pad3b;
    short min_x;
    short min_y;
    short max_x;
    short max_y;
};

extern "C" {
void func_02028ba8(Owner02028ba8 *owner, Part02028ba8 *part)
{
    if (owner->part != 0) {
        delete owner->part;
        owner->part = 0;
    }
    if (owner->elems != 0) {
        func_0207dea4(owner->elems, 0x54, 8, func_02042774);
        owner->elems = 0;
    }
    if (owner->handle != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            func_02007b70(func_02007bd8())->slot4(owner->handle);
            owner->handle = 0;
        }
    }
    owner->min_x = 0x7fff;
    owner->min_y = 0x7fff;
    owner->max_x = -0x8000;
    owner->max_y = -0x8000;
    owner->part = part;
    owner->dirty = 0;
    owner->active = 1;
}
}

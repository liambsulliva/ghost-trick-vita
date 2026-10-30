extern "C" {
extern void func_02029a04(void *object);
extern void func_02031270(void *object);
extern void *func_02042b20(void *object);
extern void func_02042774(void);
extern void func_0207dea4(void *array, int size, int count, void (*dtor)(void));
extern void *func_02007bd8(void);
}

class Manager02028b28 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(void *handle);
};

extern "C" {
extern Manager02028b28 *func_02007b70(void *object);
}

class Part02028b28 {
public:
    ~Part02028b28() { func_02029a04(this); }
    static void operator delete(void *object) { func_02031270(object); }
};

struct Elem02028b28 {
    char data[0x54];
};

struct Owner02028b28 {
    unsigned int count;
    int pad4;
    Elem02028b28 *elems;
    Part02028b28 *part;
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
Owner02028b28 *func_02028b28(Owner02028b28 *owner)
{
    unsigned int i;

    if (owner->elems != 0) {
        for (i = 0; i < owner->count; i++) {
            func_02042b20(&owner->elems[i]);
        }
        if (owner->elems != 0) {
            func_0207dea4(owner->elems, 0x54, 8, func_02042774);
            owner->elems = 0;
        }
    }
    if (owner->part != 0) {
        delete owner->part;
        owner->part = 0;
    }
    if (owner->handle != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            func_02007b70(func_02007bd8())->slot4(owner->handle);
            owner->handle = 0;
        }
    }
    return owner;
}
}

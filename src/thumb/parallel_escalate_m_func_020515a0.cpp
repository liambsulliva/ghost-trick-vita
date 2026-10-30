class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int value);
};

class Factory {
public:
    virtual Object *slot0();
};

extern "C" {
extern char data_020cf898;
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Object *object, int layer, int flags);
extern int func_02008828(Object *object, void *data);
extern void func_020153d8(Object *kid, void *owner);
}

extern "C" void func_020515a0(char *self, Factory *factory)
{
    Object *child;
    child = *(Object **)(self + 0xb4);
    if (child == 0) {
        if (factory != 0) {
            child = factory->slot0();
            *(Object **)(self + 0xb4) = child;
            if (child != 0) {
                func_0202f7e8(func_02009900(), child, 6, 0);
                if (func_02008828(*(Object **)(self + 0xb4), &data_020cf898) != 0) {
                    *(int *)(*(char **)(self + 0xb4) + 0xe0) = *(int *)(self + 0xfc);
                }
                func_020153d8(*(Object **)(self + 0xb4), self);
            }
        }
    } else if (factory == 0) {
        child->slot8(0);
    }
}

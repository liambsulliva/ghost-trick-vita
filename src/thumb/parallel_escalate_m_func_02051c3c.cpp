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

extern "C" {
extern char data_020cedc4;
extern char data_020cee88;
extern char data_020ceec0;
extern void *func_02009900(void);
extern int func_0202fa8c(void *system, void *data);
extern void func_02051cd4(void *data);
extern void *func_020507c4(void *heap);
extern void func_0202f7e8(void *system, void *object, int layer, int flags);
}

extern "C" void func_02051c3c(Object *self)
{
    void *child;
    switch (*((unsigned char *)self + 0x10)) {
    case 0:
        if (func_0202fa8c(func_02009900(), &data_020cedc4) == 0 &&
            func_0202fa8c(func_02009900(), &data_020cee88) == 0) {
            *((unsigned char *)self + 0x10) = 1;
        }
        break;
    case 1:
        func_02051cd4(&data_020ceec0);
        child = func_020507c4(&data_020cedc4);
        if (child != 0) {
            func_0202f7e8(func_02009900(), child, 4, 0);
        }
        self->slot8(0);
        break;
    }
}

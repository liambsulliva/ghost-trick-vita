class Self {
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
};
extern "C" {
extern void *func_02031254(int size);
extern void *func_0201bce4(void *object);
extern void *func_0201003c(void);
extern int func_02043fa0(void *object);
extern void func_02015b24(void *owner, int value, void *child, int param_4, int param_5);
extern void func_0201c01c(void *object);
extern unsigned char func_02043fa4(void *object);
extern void *func_02042b20(void *object);
void func_02043f30(Self *self, int param_2, int param_3)
{
    unsigned char *p = (unsigned char *)self;
    void *owner;
    void *child;

    if (*(void **)(p + 0xd0) == 0) {
        child = func_02031254(0x14);
        if (child) child = func_0201bce4(child);
        *(void **)(p + 0xd0) = child;
    }
    owner = func_0201003c();
    func_02015b24(owner, func_02043fa0(p + 0xd8), *(void **)(p + 0xd0), param_2, param_3);
    func_0201c01c(*(void **)(p + 0xd0));
    if (func_02043fa4(*(void **)(p + 0xd0)) != 0) {
        func_02042b20(p + 0xd8);
        self->slot11();
    }
}
}

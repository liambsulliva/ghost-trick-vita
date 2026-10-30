class Obj {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
};

extern "C" {
extern unsigned char func_02010b54(int object);
extern unsigned int func_020266dc(int param_1, int param_2);
extern unsigned int *func_02008444(void);
extern unsigned int func_02010194(int object);

void func_02059128(Obj *self)
{
    unsigned int found;
    unsigned int cur;
    unsigned int i;

    if (func_02010b54((int)self)) {
        for (i = 0; i < *((unsigned char *)self + 0xbe); i = i + 1) {
            found = func_020266dc((int)self, i);
            cur = func_02010194((int)func_02008444());
            if (cur == found) {
                self->v34();
                return;
            }
        }
        self->v38();
    }
}
}

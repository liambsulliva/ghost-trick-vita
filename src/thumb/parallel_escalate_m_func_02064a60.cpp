class Obj02064a60 {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual int m16();
    virtual void m17(int a, int b);
};

extern "C" {
extern int *func_02009c1c(void);
extern void func_0202f1bc(int *table, int a, int b, int c, int d);

void func_02064a60(Obj02064a60 *param_1, int param_2)
{
    param_1->m17(0x82, param_2);
    if (param_1->m16() != 0) {
        if (param_2 == 0) {
            func_0202f1bc(func_02009c1c(), ((int *)param_1)[0xe], 0, 0x8000, 0xb);
            func_0202f1bc(func_02009c1c(), ((int *)param_1)[0xf], 0, 0x800, 0xf);
            return;
        }
        if (param_2 == 1) {
            func_0202f1bc(func_02009c1c(), ((int *)param_1)[0xe], 0, 0x8000, 0x17);
            func_0202f1bc(func_02009c1c(), ((int *)param_1)[0xf], 0, 0x800, 0x1b);
        }
    }
    return;
}
}

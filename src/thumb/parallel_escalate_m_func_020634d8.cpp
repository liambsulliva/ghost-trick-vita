class Obj020634d8 {
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
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void m33(unsigned short a);
};

struct Table020634d8 {
    unsigned short v[5];
};

extern "C" {
extern const Table020634d8 data_0209dcfc;
extern const Table020634d8 data_0209dd08;
extern void func_0204c540(void *object, int value);
extern void func_02063294(int *param_1, int param_2);

void func_020634d8(int *param_1, int param_2)
{
    Table020634d8 a;
    Table020634d8 b;

    if (param_2 < 0) {
        param_2 = 0;
    }
    if (param_2 > 4) {
        param_2 = 4;
    }
    a = data_0209dcfc;
    b = data_0209dd08;
    func_02063294(param_1, param_2);
    func_0204c540((void *)param_1[0x27], b.v[param_2]);
    ((Obj020634d8 *)param_1[0x27])->m33(a.v[param_2]);
}
}

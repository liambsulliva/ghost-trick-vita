class Obj {
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
    virtual void s33(int a);
};

union Tbl {
    int v[54];
};

extern "C" {
extern const Tbl data_0209a1fc;
extern unsigned int func_02058bac(int p);
extern void func_02058a80(int *out, int p);
extern int func_0207b694(int a, int b);
extern int func_0201f110(int a, int b, int c, int d);
extern int func_0201f138(int a, int b, int c, int d);
void func_02034b3c(Obj *p, int param_2, int param_3, bool param_4, int *param_5, int *param_6, int *param_7, int *param_8, int *param_9, int *param_10, int *param_11);
}
void func_02034b3c(Obj *p, int param_2, int param_3, bool param_4, int *param_5, int *param_6, int *param_7, int *param_8, int *param_9, int *param_10, int *param_11)
{
    Tbl tbl;
    int out[3];
    unsigned int n;
    int a;
    int b;
    int c;
    int d;

    tbl = data_0209a1fc;
    n = func_02058bac(param_2);
    if (n > 0x1a) {
        n = 0x1a;
    }
    func_02058a80(out, param_2);
    ((int *)p)[0x12] = out[0];
    ((int *)p)[0x13] = out[1];
    ((int *)p)[0x14] = 0;
    ((int *)p)[0x15] = param_3;
    if (param_4) {
        ((int *)p)[0x16] = func_0207b694(tbl.v[n * 2 + 1] << 12, 0x60);
        ((int *)p)[0x17] = func_0207b694(tbl.v[n * 2] << 12, 0x60);
    } else {
        ((int *)p)[0x16] = func_0207b694(tbl.v[n * 2] << 12, 0x60);
        ((int *)p)[0x17] = func_0207b694(tbl.v[n * 2 + 1] << 12, 0x60);
    }
    b = *param_5;
    ((int *)p)[0x1a] = b;
    a = *param_6;
    ((int *)p)[0x1b] = a;
    a = func_0201f110(((int *)p)[0x16], ((int *)p)[0x17], (int)0x80000000, a - b);
    ((int *)p)[0x18] = a;
    a = func_0201f138(((int *)p)[0x16], ((int *)p)[0x17], a, ((int *)p)[0x1b] - ((int *)p)[0x1a]);
    ((int *)p)[0x19] = a;
    a = *param_7;
    ((int *)p)[0x1c] = a;
    b = *param_8;
    ((int *)p)[0x1d] = b;
    c = *param_10;
    ((int *)p)[0x20] = c;
    d = *param_11;
    ((int *)p)[0x21] = d;
    a = func_0201f110(a, b, *param_9, d - c);
    ((int *)p)[0x1e] = a;
    a = func_0201f138(((int *)p)[0x1c], ((int *)p)[0x1d], a, ((int *)p)[0x21] - ((int *)p)[0x20]);
    ((int *)p)[0x1f] = a;
    p->s33(0x911);
}

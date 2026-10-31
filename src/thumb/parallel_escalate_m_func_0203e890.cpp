class Obj3e890 {
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
};
extern "C" {
extern char data_020ce144;
extern char data_0209adbc;
extern char data_0209adc0;
extern char data_0209adcc;
extern char data_0209add4;
extern char data_0209addc;
extern char data_0209ade4;
extern int func_02008444(void);
extern void func_0200f084(int p, int a, int b, int c, int d);
extern void func_0200ec24(int p, int a);
extern int func_02022dc8(int *p);
extern int func_02008828(int *p, void *q);
extern void func_0200da04(void *a, void *b, void *c, void *d);
extern int func_02012518(int p, void *a, void *b);
extern void func_02031fe8(int p);
extern void func_02022f6c(int *p);

void func_0203e890(int *param_1)
{
    switch (*(unsigned char *)(param_1 + 1)) {
    case 0:
        func_0200f084(func_02008444(), 0, 1, 0, 1);
        *(unsigned char *)(param_1 + 1) = *(unsigned char *)(param_1 + 1) + 1;
        return;
    case 1:
        func_0200ec24(func_02008444(), 0);
        ((Obj3e890 *)param_1)->s11();
        *(unsigned char *)(param_1 + 1) = *(unsigned char *)(param_1 + 1) + 1;
        return;
    case 2:
        if (func_02022dc8(param_1) != 0) {
            return;
        }
        if (func_02008828(param_1, &data_020ce144) != 0) {
            func_0200da04(&data_0209adbc, &data_0209adc0, &data_0209adcc, &data_0209add4);
            *(unsigned char *)(param_1 + 1) = 99;
            return;
        }
        if (func_02012518(func_02008444(), &data_0209addc, &data_0209ade4) != 0) {
            func_0200da04(&data_0209adbc, &data_0209addc, &data_0209ade4, &data_0209add4);
            func_02031fe8(func_02008444());
            *(unsigned char *)(param_1 + 1) = 99;
            return;
        }
        func_02022f6c(param_1);
        return;
    case 99:
        return;
    }
}
}

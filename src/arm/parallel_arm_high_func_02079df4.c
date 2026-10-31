extern unsigned int func_02075c98(void);
extern void func_02075cac(unsigned int bits);
extern int func_02079b48(void);
extern void func_02074fbc();
extern void func_02074f7c();
extern void func_02074f9c();
extern void func_02074fd0();
extern void func_02074810();
extern void func_020748a0();
extern void func_02074f8c();
extern void func_02074fac();
extern void func_02074fe0();

void func_02079df4(int param_1)
{
    unsigned int saved;
    unsigned int uVar2;

    saved = func_02075c98();
    *(int *)0x020d2efc = param_1;
    if (func_02079b48()) {
        uVar2 = 0x1000;
        if (param_1 == 0) {
            uVar2 = 0x5000;
        }
        func_02074fbc(0xf000, uVar2);
        if (param_1 != 0) {
            func_02074810();
            func_020748a0();
            func_02074f8c(8);
            func_02074fac(8);
            func_02074fe0(8);
        } else {
            func_02074f7c(8);
            func_02074f9c(8);
            func_02074fd0(8);
        }
    }
    func_02075cac(saved);
}

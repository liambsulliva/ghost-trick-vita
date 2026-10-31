typedef int code();
extern char data_020a7e94[];
extern char data_020ce820;
extern void func_0202f64c(char *p);
extern void func_0202fbb0(int *it);
extern int func_0200eac4(int *it);
extern int *func_0200eab4(int *it);
extern int *func_0200eab8(int *it);
extern void func_0200eabc(int *it, int v);
extern int func_02008828(int *p, void *q);
extern void func_02043a80(int *p, int v);
extern void func_02043b8c(int *p, int v);
extern void func_0202f5e4(int *p);
extern void func_0202f644(int *p);

int *func_02042fb0(int *param_1)
{
    int aBlk[4];
    char *pB;
    char *pA;
    char *p0;
    unsigned int uVar4;
    int *piVar2;
    char *uVar1;
    int iZero;

    *param_1 = (int)data_020a7e94;
    p0 = (char *)param_1;
    pA = (char *)param_1;
    pA = pA + 0x4c;
    pB = (char *)param_1;
    pB = pB + 0x50;
    p0 = p0 + 0x48;
    uVar4 = 0;
    do {
        func_0202f64c(p0 + uVar4 * 16);
        func_0202f64c(pA + uVar4 * 16);
        func_0202f64c(pB + uVar4 * 16);
        uVar4 = uVar4 + 1;
    } while (uVar4 < 5);
    func_0202f64c((char *)(param_1 + 0xf));
    func_0202f64c((char *)(param_1 + 0x10));
    func_0202f64c((char *)(param_1 + 0xe));
    func_0202f64c((char *)(param_1 + 0x11));
    func_0202fbb0(aBlk);
    uVar1 = &data_020ce820;
    iZero = 0;
    while (func_0200eac4(aBlk) != 0) {
        if (func_02008828(func_0200eab4(aBlk), uVar1) != 0) {
            piVar2 = func_0200eab8(aBlk);
            (**(code **)(*piVar2 + 0x28))(piVar2);
        }
        func_0200eabc(aBlk, iZero);
    }
    func_02043a80(param_1, 0);
    func_02043b8c(param_1, 1);
    func_0202f5e4(param_1);
    func_0202f644(param_1);
    return param_1;
}

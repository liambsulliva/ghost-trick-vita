extern char data_02085688;
extern char data_020a5408;
extern int func_02007aa8();
extern int func_02007bd8();
extern int func_0200c530();
extern int func_0201e2cc();
extern int func_0201e2ec();
extern int func_0201f288();
extern int func_0202f7dc();
extern int func_0207dd08();

unsigned int * func_0202f76c(unsigned int *param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;

    func_02007aa8();
    *param_1 = ((unsigned int)&data_020a5408);
    func_0201e2cc(param_1 + 1);
    func_0207dd08(param_1 + 9,0x10,0x10,((unsigned int)0x0202fc65u),((unsigned int)0x0200995du));
    uVar2 = 0;
    func_0201e2ec(param_1 + 1,0x180,0x110,0,0);
    func_0200c530(param_1 + 1,((unsigned int)&data_02085688));
    uVar1 = func_02007bd8();
    func_0201f288(uVar1,param_1 + 1);
    do {
        param_1[uVar2 * 4 + 0xc] = uVar2;
        uVar2 = uVar2 + 1;
    } while (uVar2 < 0x10);
    func_0202f7dc(param_1);
    return param_1;
}

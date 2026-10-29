extern int func_02069bc0();
extern int func_02072d88();
extern int func_02073594();
extern int func_02073620();
extern int func_020736c0();
extern int func_02073a04();
extern int func_0207511c();
extern int func_02075f84();

void func_02075dc0(unsigned int param_1)

{
    unsigned int *puVar1;
    unsigned short uVar2;
    unsigned int uVar3;

    if (*((unsigned short *)0x02fffc40u) == 2) {
        func_02075f84();
    }
    uVar2 = func_02073a04();
    func_02069bc0(uVar2);
    func_02073594(0x40000);
    func_02073620(0xffffffff);
    func_02072d88();
    puVar1 = ((unsigned int *)0x02fffc20u);
    *((unsigned int *)0x02fffc20u) = param_1;
    func_020736c0(0x10);
    uVar3 = func_0207511c();
    puVar1[0xdf] = uVar3;
    func_01ff837c();
    return;
}

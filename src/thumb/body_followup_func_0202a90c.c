extern int func_0202a940();
extern int func_0202ace0();
extern unsigned long long func_0202af14();

int func_0202a90c(int param_1,int param_2)

{
    unsigned int uVar1;
    unsigned long long uVar2;

    if (*(char *)(param_1 + 0x24) == '\x01') {
        return 0xff;
    }
    uVar2 = func_0202af14(param_1,param_2);
    if ((int)uVar2 == 0xff) {
        return (int)uVar2;
    }
    uVar1 = func_0202ace0(param_1,param_2);
    func_0202a940(param_1,uVar1,param_2);
    return (int)uVar2;
}

extern int func_02010d18();
extern int func_02013514();
extern int func_0201354c();
extern int func_020790c8();

void func_02013518(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02013514(param_3);
    iVar2 = func_020790c8(uVar1,((unsigned int)0x0208412cu));
    if (iVar2 == 0) {
        func_0201354c(param_2,0);
        return;
    }
    func_0201354c(param_2,1);
    func_02010d18(param_2);
    return;
}

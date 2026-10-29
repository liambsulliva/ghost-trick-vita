extern int func_0207682c();
extern int func_0207705c();

void func_0207652c(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = 0;
    for (uVar2 = param_3; (iVar1 < 8 && (uVar2 != 0)); uVar2 = uVar2 >> 1) {
        if ((uVar2 & 1) != 0) {
            func_0207705c(iVar1);
        }
        iVar1 = iVar1 + 1;
    }
    func_0207682c(0xd,param_1,param_2,param_3,param_4);
    return;
}

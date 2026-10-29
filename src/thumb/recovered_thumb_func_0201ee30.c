extern int func_0201ee68();
extern int func_0201ee90();
extern int func_020790c8();

unsigned int func_0201ee30(int param_1,unsigned int param_2)

{
    int iVar1;
    unsigned int uVar2;

    if (param_1 == 0) {
        return 0;
    }
    iVar1 = func_0201ee68(param_2);
    if (iVar1 != 0) {
        uVar2 = func_0201ee68(param_2);
        iVar1 = func_020790c8(uVar2,param_1);
        if (iVar1 == 0) {
            return param_2;
        }
    }
    uVar2 = func_0201ee90(param_2,param_1);
    return uVar2;
}

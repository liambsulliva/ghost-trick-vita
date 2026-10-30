extern void func_02078068();
extern int func_02077fe4();
extern int func_020783c8();

int func_02078410(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;
    int iStack_c;
    unsigned int uStack_8;

    uStack_8 = param_4;
    iVar1 = func_020783c8(param_1,param_2,param_3,((unsigned int)((unsigned int)func_02078068)),&iStack_c);
    if (iVar1 == 0) {
        func_02077fe4();
        return iStack_c;
    }
    return iVar1;
}

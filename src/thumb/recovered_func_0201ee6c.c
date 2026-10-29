extern int func_0201ee8c();
extern int func_0201eec0();

unsigned int func_0201ee6c(int param_1,unsigned int param_2)

{
    int iVar1;

    iVar1 = func_0201ee8c(param_2);
    if (param_1 != iVar1) {
        param_2 = func_0201eec0(param_2,param_1);
    }
    return param_2;
}

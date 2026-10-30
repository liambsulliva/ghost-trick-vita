typedef int code();
extern int func_0206b350();
extern int func_0207303c();

int func_0206ce1c(int *param_1,unsigned short *param_2,unsigned int param_3)
{
    int iVar1;
    int iVar2;

    iVar2 = *param_1;
    if (*(int *)(*(int *)(iVar2 + 0x20) + 0x1c) != 0)
    {
        func_0207303c((unsigned short *)param_1[1],param_2,param_3);
        iVar2 = 0;
    }
    else
    {
        iVar1 = (**(code **)(*(int *)(iVar2 + 0x20) + 0x20))(iVar2,param_2,param_1[1],param_3);
        iVar2 = func_0206b350(*(int *)(iVar2 + 8),iVar1);
    }
    param_1[1] = param_1[1] + param_3;
    return iVar2;
}

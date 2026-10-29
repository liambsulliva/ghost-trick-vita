void func_02010264(int param_1,int param_2)

{
    if (param_2 < 0) {
        param_2 = 300;
    }
    *(int *)(param_1 + ((int)0x00000d8cu)) = param_2;
    return;
}

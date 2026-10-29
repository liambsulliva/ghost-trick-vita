void func_0206e0b4(int param_1,int param_2,int param_3)

{
    if (param_3 == 0) {
        param_2 = 0;
    }
    else if (param_2 == 0) {
        param_3 = 0;
    }
    *(int *)(param_1 + 0x54) = param_2;
    *(int *)(param_1 + 0x58) = param_3;
    return;
}

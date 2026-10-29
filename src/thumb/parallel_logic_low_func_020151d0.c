typedef int code();

void func_020151d0(int *param_1,int param_2)

{
    if (param_2 != 0) {
        (**(code **)(*param_1 + 0x28))(param_1);
        return;
    }
    (**(code **)(*param_1 + 0x2c))(param_1);
    return;
}

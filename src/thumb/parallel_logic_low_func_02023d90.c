typedef int code();

void func_02023d90(int *param_1,int param_2)
{
    (**(code **)(*param_1 + 0x10))(param_1,param_2);
}

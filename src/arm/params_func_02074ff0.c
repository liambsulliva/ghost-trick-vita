extern char data_020b1768;
typedef int code();

void func_02074ff0(int param_1,unsigned int param_2)

{
    (**(code **)(((int)&data_020b1768) + param_1 * 4))(param_2);
    return;
}

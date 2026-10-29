typedef int code();

void func_0203b190(int *param_1,unsigned int param_2)

{
    param_1[0x3a] = param_2;
    (**(code **)(*param_1 + 0xa8))(param_1,param_2 & 0xffff,0,1,0,1,0xffffffff,0);
    return;
}

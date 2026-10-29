extern int func_0200f57c();

void func_020330e0(int param_1,int param_2)

{
    *(int *)(param_1 + ((int)0x00000d94u)) = *(int *)(param_1 + ((int)0x00000d94u)) + param_2;
    func_0200f57c(param_1);
    *(unsigned int *)(param_1 + 8) = *(unsigned int *)(param_1 + 8) | 0x80;
    return;
}

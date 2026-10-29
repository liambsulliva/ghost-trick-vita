extern int func_02014a8c();

void func_02015a54(int param_1,unsigned int param_2,unsigned int param_3)

{
    func_02014a8c(param_1 + ((unsigned int)0x00000f04u),param_2,0,param_3);
    *(unsigned int *)(param_1 + 8) = *(unsigned int *)(param_1 + 8) | 4;
    return;
}

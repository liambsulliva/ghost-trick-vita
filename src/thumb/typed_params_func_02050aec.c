extern char data_02027edd;
void func_02050aec(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    *(unsigned int *)(param_1 + 0x38) = param_2;
    *(unsigned int *)(param_1 + 0x34) = param_4;


    func_02027edc(param_1,param_3,param_4);
    return;
}

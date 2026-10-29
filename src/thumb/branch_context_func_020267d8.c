extern int func_02014b70();
extern int func_02014b80();

void func_020267d8(int param_1,unsigned int param_2,int param_3)

{
    if (param_3 != 0) {
        func_02014b80(param_1 + ((int)0x00000f04u),param_2);
        return;
    }
    func_02014b70(param_1 + ((int)0x00000f04u),param_2);
    return;
}

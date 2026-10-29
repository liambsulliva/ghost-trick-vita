extern int func_0200853c();
extern int func_02008734();

void func_02007c40(int param_1,unsigned int param_2)

{
    *(unsigned int *)(param_1 + 4) = param_2;
    func_02008734();
    func_0200853c(param_1,*(unsigned short *)(param_1 + 0xe));
    return;
}

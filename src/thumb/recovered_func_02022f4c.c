extern int func_02022f2c();
extern int func_02022f6c();

void func_02022f4c(int param_1)

{
    if (*(int *)(param_1 + 4) != 0) {
        func_02022f6c();
    }
    func_02022f2c(param_1,((unsigned int)0x020c9de8u),0);
    return;
}

extern int func_0200bc64();

void func_0205953c(int param_1,int param_2)

{
    *(char *)(param_1 + 0xc3) = (char)param_2;
    if ((param_2 == 0) || (param_2 != 9)) {
        func_0200bc64(param_1 + 0x28,0);
        return;
    }
    func_0200bc64(param_1 + 0x28,((unsigned int)0x020cf828u));
    return;
}

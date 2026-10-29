extern int func_0206fd50();
extern int func_0206fe48();

void func_ov004_020d5b78(int param_1)

{
    if (*(int *)(param_1 + 0x48) == 0) {
        func_0206fe48();
        return;
    }
    func_0206fd50();
    return;
}

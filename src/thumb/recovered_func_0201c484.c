extern int func_0201c04c();
extern int func_0201c350();
extern int func_0201c384();

void func_0201c484(int param_1)

{
    func_0201c350();
    func_0201c384(param_1);
    if (*(int *)(param_1 + 0x28) != 0) {
        func_0201c04c(*(int *)(param_1 + 0x28),0x1f);
    }
    return;
}

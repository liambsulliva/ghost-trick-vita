extern int func_0201786c();
extern int func_0201bc84();

void func_0201bb98(int param_1)

{
    if (*(int *)(param_1 + 0x68) != 0) {
        func_0201786c(*(int *)(param_1 + 0x68),*(unsigned int *)(param_1 + 0x78));
    }
    if (*(int *)(param_1 + 0x6c) != 0) {
        func_0201786c(*(int *)(param_1 + 0x6c),*(unsigned int *)(param_1 + 0x78));
    }
    func_0201bc84(param_1);
    return;
}

extern int func_02031254();
extern int func_02028b08();
extern int func_02028ba8();
extern int func_02028c34();
extern int func_02029780();

void func_02029744(int param_1, unsigned int param_2, unsigned int param_3, unsigned char param_4)
{
    int iVar1;

    iVar1 = func_02031254(0x44);
    if (iVar1 != 0) {
        iVar1 = func_02028b08(iVar1);
    }
    *(int *)(param_1 + 0x48) = iVar1;
    func_02028ba8(iVar1, param_3);
    func_02028c34(*(int *)(param_1 + 0x48), param_2, param_4, 0, 1);
    func_02029780(*(int *)(param_1 + 0x48), 1);
    return;
}

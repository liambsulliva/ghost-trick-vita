extern int func_0201b9f0();

void func_0201b718(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;

    iVar1 = *(int *)(param_1 + 0x54);
    if (iVar1 != 1) {
        if (iVar1 == 2) {
            *(char *)(param_1 + 0x4e) = *(char *)(param_1 + 0x4e) + '\x01';
            return;
        }
        func_0201b9f0(param_1,iVar1,param_3,param_4);
        *(char *)(param_1 + 0x3b) = *(char *)(param_1 + 0x3b) + '\x01';
    }
    return;
}

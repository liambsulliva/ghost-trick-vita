extern int func_02019a58();
extern int func_0201e074();

unsigned int func_02019b18(int param_1)

{
    int iVar1;

    iVar1 = func_02019a58(*(unsigned int *)(param_1 + 0x68),*(unsigned char *)(param_1 + 0x70));
    if (iVar1 == 1) {
        *(unsigned short *)(param_1 + 0x44) = *(unsigned short *)(*(int *)(param_1 + 0x14) + 2);
    }
    func_0201e074(*(unsigned int *)(param_1 + 0x6c),*(unsigned char *)(param_1 + 0x70),iVar1,
                              *(unsigned short *)(*(int *)(param_1 + 0x14) + 2));
    return 0;
}

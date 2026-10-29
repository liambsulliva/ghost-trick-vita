extern int func_02019a58();
extern int func_0201e09c();

unsigned int func_02019a30(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02019a58(*(unsigned int *)(param_1 + 0x68),*(unsigned char *)(param_1 + 0x70));
    func_0201e09c(*(unsigned int *)(param_1 + 0x6c),*(unsigned char *)(param_1 + 0x70),uVar1,
                              *(unsigned short *)(*(int *)(param_1 + 0x14) + 2));
    return 0;
}

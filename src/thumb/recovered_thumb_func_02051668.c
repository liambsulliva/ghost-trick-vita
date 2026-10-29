extern char data_0209c17c;
extern char data_0209c184;
extern char data_0209c190;
extern int func_0200da04();
extern int func_02027c28();
extern int func_02050ec0();
extern int func_020516b8();

unsigned int func_02051668(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02027c28(*(unsigned short *)(*(int *)(param_1 + 4) + *(int *)(param_1 + 8) * 2 + 8));
    func_020516b8(*(unsigned int *)(param_1 + 0x34),uVar1);
    func_02050ec0(*(unsigned int *)(param_1 + 0x34),*(unsigned int *)(param_1 + 0x30),
                                *(unsigned int *)(param_1 + 4),*(int *)(param_1 + 8) + 5);
    uVar1 = ((unsigned int)&data_0209c17c);
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 1;
    func_0200da04(*(unsigned int *)(*(int *)(param_1 + 0x30) + 0xfc),uVar1,((unsigned int)&data_0209c184),((unsigned int)&data_0209c190));
    return 1;
}

extern int func_02009a0c();
extern int func_0200a478();
extern int func_0200a620();
extern int func_0200a6a8();

void func_02009968(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_02009a0c((unsigned char *)(param_1 + ((int)0x00000202u)),0x6000);
    func_02009a0c((unsigned char *)(param_1 + ((int)0x00006202u)),0xc00);
    func_02009a0c((unsigned char *)(param_1 + ((int)0x00006e02u)),0xc00);
    func_0200a478(param_1);
    uVar2 = 0;
    do {
        func_0200a6a8((int *)(param_1 + uVar2 * 0x10));
        iVar1 = ((int)0x0000aa28u);
        uVar2 = uVar2 + 1;
    } while (uVar2 < 0x10);
    *(unsigned short *)(param_1 + ((int)0x0000aa28u)) = 0;
    *(unsigned short *)(param_1 + iVar1 + 2) = 0;
    *(unsigned char *)(param_1 + iVar1 + 0x14) = 0xff;
    *(unsigned char *)(param_1 + iVar1 + 0x15) = 0xff;
    *(short *)(param_1 + iVar1 + 0xc) = -1;
    *(short *)(param_1 + iVar1 + 0xe) = -1;
    *(unsigned short *)(param_1 + iVar1 + 0x10) = 0;
    *(unsigned short *)(param_1 + iVar1 + 0x12) = 0;
    *(unsigned char *)(param_1 + iVar1 + 0x18) = 0;
    *(unsigned char *)(param_1 + iVar1 + 0x1a) = 0;
    *(unsigned short *)(param_1 + iVar1 + 6) = 0;
    *(unsigned char *)(param_1 + iVar1 + 0x1b) = 0;
    *(unsigned int *)(param_1 + iVar1 + -4) = 0;
    func_0200a620(param_1 + 0x100);
    return;
}

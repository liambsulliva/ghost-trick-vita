extern int func_02022dfc();
extern int func_02022f34();
extern int func_ov003_020d3c14();
extern int func_ov003_020d4184();
extern int func_ov003_020d4d50();
extern int func_ov003_020d4f70();

void func_ov003_020d5740(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_ov003_020d3c14();
    *(unsigned int *)(param_1 + 0x17c) = 0;
    *(unsigned int *)(param_1 + 0x178) = 0;
    if (*(char *)(param_1 + 0x170) != '\0') {
        *(unsigned char *)(param_1 + 0x16e) = 0;
    }
    iVar1 = ((unsigned int)0x0000016eu);
    *(unsigned char *)(param_1 + ((unsigned int)0x0000016eu) + 0x1a) =
              *(unsigned char *)(param_1 + ((unsigned int)0x0000016eu));
    func_ov003_020d4184(param_1,0xffffffff);
    func_ov003_020d4f70(param_1);
    *(unsigned int *)(param_1 + iVar1 + 6) = 0x1c;
    func_ov003_020d4d50(param_1,0x1c,5);
    uVar2 = func_02022dfc(param_1);
    func_02022f34(uVar2,((unsigned int)0x020d5aecu),
                              (unsigned int)*(unsigned char *)(param_1 + iVar1 + 1) << 0x10 |
                              (unsigned int)*(unsigned char *)(param_1 + iVar1 + -1) << 8 | *(unsigned char *)(param_1 + iVar1) + 1);
    return;
}

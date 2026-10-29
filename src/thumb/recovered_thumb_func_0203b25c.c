extern int func_02010b60();
extern int func_02010b64();
extern int func_02010b7c();

unsigned int func_0203b25c(int param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;

    func_02010b7c();
    uVar1 = func_02010b60();
    func_02010b7c();
    uVar2 = func_02010b64();
    if ((((*(unsigned short *)(param_1 + 10) < uVar1) &&
              ((int)uVar1 < (int)(*(unsigned short *)(param_1 + 10) + 0x16))) &&
            (*(unsigned short *)(param_1 + 0xc) < uVar2)) &&
          ((int)uVar2 < (int)(*(unsigned short *)(param_1 + 0xc) + 0x16))) {
        return 1;
    }
    return 0;
}

extern int func_02017cf8();

unsigned int func_02017cd0(unsigned char *param_1)

{
    int iVar1;

    if (!(((param_1[0xa8] == '\0') || (param_1[0x8c] != '\0')) ||
          (iVar1 = func_02017cf8(param_1), iVar1 != 0))) {
        return 1;
    }
    return 0;
}

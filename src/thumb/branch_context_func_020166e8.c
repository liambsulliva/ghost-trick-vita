extern int func_020151f0();
extern int func_02016710();

unsigned int func_020166e8(unsigned char *param_1)

{
    int iVar1;

    iVar1 = func_020151f0(param_1);
    if (iVar1 != 0) {
        return 0;
    }
    if ((*(unsigned char **)(param_1 + 4) != (unsigned char *)0x0) && (iVar1 = func_02016710(*(unsigned char **)(param_1 + 4)), iVar1 != 0)) {
        return 1;
    }
    return 0;
}

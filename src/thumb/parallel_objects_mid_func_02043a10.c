extern int func_02043a38(unsigned char *object);

int func_02043a10(int param_1)
{
    unsigned int i;
    unsigned char *pbVar1;

    for (i = 0; i < 5; i++) {
        pbVar1 = *(unsigned char **)(param_1 + i * 0x10 + 0x48);
        if (pbVar1 != (unsigned char *)0x0) {
            if (func_02043a38(pbVar1) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

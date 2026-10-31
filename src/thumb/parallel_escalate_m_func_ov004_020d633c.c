void func_ov004_020d5c10(char *param_1);

void func_ov004_020d633c(char *param_1)
{
    unsigned int i;

    if ((((unsigned int)*(unsigned char *)(param_1 + 0xaa8) << 25) >> 31) == 0) {
        for (i = 0; i < *(unsigned int *)(param_1 + 0xdc) - 1; i++) {
            func_ov004_020d5c10(param_1);
        }
    }
    do {
    } while ((((unsigned int)*(unsigned char *)(param_1 + 0xaa9) << 24) >> 31) == 0);
}

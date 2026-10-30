extern int func_0201e418();

unsigned char *func_0201e3dc(int *param_1)
{
    unsigned char *puVar1;

    if (param_1[3] == 0) {
        return (unsigned char *)0x0;
    }
    puVar1 = (unsigned char *)param_1[4];
    if (puVar1 != (unsigned char *)0x0) {
        param_1[4] = *(int *)(puVar1 + ((unsigned int)param_1[2] & ~3) - 4);
        if (*((char *)param_1 + 0x19) == '\0') {
            func_0201e418(puVar1,param_1[2]);
        }
        param_1[7] = param_1[7] + 1;
    }
    return puVar1;
}

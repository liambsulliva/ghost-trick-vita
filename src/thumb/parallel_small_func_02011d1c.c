void func_02011d1c(char *param_1)
{
    unsigned int i;
    int *slot;
    int *owner;

    for (i = 0; i < (unsigned char)param_1[0xf20]; i++) {
        slot = (int *)param_1 + i;
        owner = (int *)slot[0x3c9];
        if (owner == 0) {
            goto next;
        }
        if (owner == 0) {
            goto clear;
        }
        ((void (*)(int *))(*(int *)(*owner + 4)))(owner);
clear:
        slot[0x3c9] = 0;
next:
        ;
    }
    param_1[0xf20] = 0;
    param_1[0xf21] = 4;
}

extern int func_02017e78();
extern int func_020183d0();
extern int func_0201c270();

unsigned int func_020183e4(unsigned char *param_1)

{
    int iVar1;
    unsigned char *puVar2;

    param_1[0x8f] = 1;
    param_1[0xa4] = 0;
    iVar1 = func_02017e78(param_1);
    if (iVar1 != 0) {
        puVar2 = (unsigned char *)func_0201c270(((unsigned int)0x02084cfcu),(unsigned int)*(unsigned short *)(param_1 + 0xa0));
        if ((*(int *)(param_1 + 0x80) != 0) && (puVar2 != (unsigned char *)0x0)) {
            func_020183d0(puVar2,*(int *)(param_1 + 0x80));
        }
        param_1[0x9e] = 0;
    }
    return 0;
}

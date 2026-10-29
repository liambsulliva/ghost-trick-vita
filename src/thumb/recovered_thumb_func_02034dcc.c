extern char data_0209a2f0;
extern char data_0209a30c;
extern char data_0209a310;
extern char data_0209a31c;
extern char data_0209a324;
extern char data_020c9d50;
extern int func_0200da04();
extern int func_02061044();

void func_02034dcc(int param_1)

{
    int iVar1;

    if (*(char *)(param_1 + 0x4c) != '\0') {
        if ((*(char *)(param_1 + 0x4d) == '\0') || (*(char *)(param_1 + 0x4d) != '\x01')) {
            iVar1 = func_02061044(*(unsigned int *)(param_1 + 0x24));
            if ((iVar1 != 0) || ((*((unsigned int *)&data_020c9d50) & 0xf00) != 0)) {
                func_0200da04(((unsigned int)&data_0209a30c),((unsigned int)&data_0209a310),((unsigned int)&data_0209a31c),((unsigned int)&data_0209a2f0));
                *(unsigned char *)(param_1 + 0x4d) = 1;
                return;
            }
        }
        else {
            iVar1 = func_02061044(*(unsigned int *)(param_1 + 0x24));
            if ((iVar1 == 0) && ((*((unsigned int *)&data_020c9d50) & 0xf00) == 0)) {
                func_0200da04(((unsigned int)&data_0209a30c),((unsigned int)&data_0209a310),((unsigned int)&data_0209a324),((unsigned int)&data_0209a2f0));
                *(unsigned char *)(param_1 + 0x4d) = 0;
            }
        }
    }
    return;
}

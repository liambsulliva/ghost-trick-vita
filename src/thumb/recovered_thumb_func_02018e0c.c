extern char data_02084cfc;
extern char data_020b9d84;
extern int func_02009900();
extern int func_02017e64();
extern int func_02017e78();
extern int func_020183d0();
extern int func_020187bc();
extern int func_0201c270();
extern int func_0201c310();
extern int func_0202fb48();

unsigned int func_02018e0c(int param_1,unsigned int param_2)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_02017e64();
    if (iVar1 != 0) {
        if ((param_2 & 0x1000) != 0) {
            uVar2 = func_02009900();
            iVar1 = func_0202fb48(uVar2,((unsigned int)&data_020b9d84),0xffffffff);
            if (iVar1 != 0) {
                func_0201c310();
            }
        }
        return 1;
    }
    if ((*(char *)(param_1 + 0x8f) == '\0') && ((param_2 & 0x1000) != 0)) {
        func_020187bc(param_1);
        *(unsigned char *)(param_1 + 0x8f) = 1;
        iVar1 = func_02017e78(param_1);
        if (iVar1 != 0) {
            iVar1 = func_0201c270(((unsigned int)&data_02084cfc),*(unsigned short *)(param_1 + 0xa0));
            if ((*(int *)(param_1 + 0x80) != 0) && (iVar1 != 0)) {
                func_020183d0();
            }
            *(unsigned char *)(param_1 + 0x9e) = 0;
            return 1;
        }
    }
    return 0;
}

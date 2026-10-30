extern char data_02080e84;
extern char data_02080e86;
extern int func_0200730c();

void func_0202aba4(int param_1,int param_2,int param_3)

{
    unsigned char *pbVar1;
    int iVar2;

    pbVar1 = (unsigned char *)(param_1 + 0x1e4 + param_2 * 0xc);
    if (*(char *)(param_1 + 0x24) != '\x01') {
        func_0200730c(0xffffffff,(int)*(short *)(((int)&data_02080e86) + *(int *)(pbVar1 + 8) * 8),
                                    (int)*(short *)(((int)&data_02080e84) + *(int *)(pbVar1 + 8) * 8),param_3,param_2);
        if (param_3 != 0) {
            pbVar1[1] = 3;
            return;
        }
        iVar2 = *pbVar1;
        *pbVar1 = iVar2 & ~1;
        pbVar1[1] = 0;
        pbVar1[2] = 0;
    }
    return;
}

extern char data_020b18a0;
extern int func_02074188();

void func_020050d0(int param_1)

{
    if (*(int *)(param_1 + 0x100) - *(int *)(param_1 + 0x104) <
            (int)((unsigned int)*(unsigned short *)(param_1 + 0xfc) - (unsigned int)*(unsigned short *)(param_1 + 0xfe))) {
        if (*(int *)(*(int *)(((int)&data_020b18a0) + 0x10) + ((int)0x00004c4cu)) == 0) {
            func_02074188(*(int *)(((int)&data_020b18a0) + 0x10) + ((int)0x00001ca4u) +
                                        (unsigned int)*(unsigned char *)(param_1 + 0x62) * 0xc0);
        }
        else {
            *(unsigned char *)(param_1 + 0xf4) = 1;
        }
    }
    *(unsigned int *)(param_1 + 0x104) = *(int *)(param_1 + 0x104) + (unsigned int)*(unsigned short *)(param_1 + 0xfe);
    return;
}

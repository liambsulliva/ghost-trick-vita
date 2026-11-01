extern unsigned char data_020c9e48[];
extern unsigned char data_020c9e88[];
void func_02064b20(int param_1, unsigned int param_2, int param_3)
{
    if (param_3 == 0) {
        *(unsigned int *)(param_1 + 0x60) = param_2;
        ((unsigned short *)data_020c9e48)[13] = (unsigned short)(param_2 & 0x1ff);
    } else if (param_3 == 1) {
        *(unsigned int *)(param_1 + 0x64) = param_2;
        ((unsigned short *)data_020c9e88)[19] = (unsigned short)(param_2 & 0x1ff);
    }
}

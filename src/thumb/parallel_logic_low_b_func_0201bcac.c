extern unsigned int data_020c9e48[];
extern unsigned int data_020c9e88[];

void func_0201bcac(int param_1, int param_2)
{
    if (param_2 == 1) {
        *(unsigned int *)((char *)data_020c9e48 + 0x54) = 0x00210210;
        *(unsigned short *)((char *)data_020c9e88 + 0x1a) = 0x201;
        return;
    }
    data_020c9e48[0] = data_020c9e48[0] | 0x200;
    *(unsigned short *)((char *)data_020c9e48 + 6) = 0xf09;
}

extern int func_020071bc();

void func_0202a6d8(int param_1,int param_2,int param_3,unsigned int param_4)

{
    unsigned char *pbVar1;

    pbVar1 = (unsigned char *)(param_1 + 0x38 + param_2 * 0x10);
    if (*(char *)(param_1 + 0x24) != '\x02') {
        func_020071bc(*(unsigned short *)(pbVar1 + 2),0,4,0,0,(param_3 << 0xf) >> 0x10,0,0,0,param_4);
        *pbVar1 = *pbVar1 | 2;
    }
    return;
}

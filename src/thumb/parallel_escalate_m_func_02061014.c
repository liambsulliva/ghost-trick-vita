extern int func_020082d8();
extern unsigned int data_020a46c8;

void func_02061014(unsigned int *param_1,int param_2,unsigned int param_3,unsigned int param_4)
{
    if (*(unsigned short *)(param_2 + 0xec) != 0) {
        func_020082d8(param_1,*(int *)(param_2 + 0xd0) + (*(unsigned char *)(param_2 + 0x7f) - 1) * 0xc);
        return;
    }
    func_020082d8(param_1,&data_020a46c8);
    return;
}

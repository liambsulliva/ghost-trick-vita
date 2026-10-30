extern int func_0205b960();
extern int func_0205bb00();

int func_0205b9a8(int param_1,unsigned int param_2,unsigned char param_3)
{
    *(unsigned int *)(param_1 + 0x34) = param_2;
    *(unsigned char *)(param_1 + 0x9e) = param_3;
    if (*(int *)(param_1 + 0x30) == 0) {
        func_0205bb00(param_1);
        func_0205b960(param_1);
        *(unsigned int *)(param_1 + 0x94) = 0xcd;
        *(unsigned char *)(param_1 + 0xa1) = 1;
        return 1;
    }
    *(unsigned char *)(param_1 + 0x10) = 2;
    *(unsigned char *)(param_1 + 0x9f) = 0;
    *(unsigned int *)(param_1 + 0x94) = 0xcd;
    return 0;
}

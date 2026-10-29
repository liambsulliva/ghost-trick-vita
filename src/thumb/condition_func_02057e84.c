extern char data_020ae298;
extern int func_02011774();

unsigned int * func_02057e84(unsigned int *param_1)

{
    func_02011774();
    *param_1 = ((unsigned int)&data_020ae298);
    *(unsigned char *)((int)param_1 + 0xd5) = 0;
    *(unsigned char *)((int)param_1 + 0xd6) = 0;
    *(unsigned char *)((int)param_1 + 0xd7) = 0;
    *(unsigned char *)(param_1 + 0x36) = 1;
    *(unsigned char *)((int)param_1 + 0xd9) = *(unsigned char *)((int)param_1 + 0xd9) & ~0x1;
    *(unsigned char *)((int)param_1 + 0xd9) = *(unsigned char *)((int)param_1 + 0xd9) & ~0x2;
    *(unsigned char *)((int)param_1 + 0xda) = 0;
    return param_1;
}

extern int func_02008444();
extern int func_02032008();

void func_ov003_020d5670(int param_1)

{
    *(unsigned char *)(param_1 + 0x168) = 6;
    *(unsigned int *)(param_1 + 0x174) = 0;
    func_02008444();
    func_02032008();
    return;
}

extern int func_02017760();
extern int func_0201e060();

unsigned int func_02019b68(int param_1)

{
    func_0201e060(*(unsigned int *)(param_1 + 0x6c),*(unsigned char *)(param_1 + 0x70));
    func_02017760(param_1);
    *(unsigned int *)(param_1 + 0x14) = 0;
    *(unsigned int *)(param_1 + 0x18) = 0;
    return 1;
}

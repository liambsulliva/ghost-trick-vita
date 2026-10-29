extern int func_0202c358();
extern int func_02042bb0();

void func_02056cb8(int param_1)

{
    volatile unsigned int *word;

    if (func_0202c358(param_1) == 0) {
        word = (volatile unsigned int *)(param_1 + 0x78);
        func_02042bb0((unsigned int)*(unsigned short *)(param_1 + 0x5c),(unsigned char *)(param_1 + 0x5e),(*word << 29) >> 31,(unsigned int)*(unsigned char *)(param_1 + 0x5f),(*word << 28) >> 31,(*word << 27) >> 31,*(int *)(param_1 + 0x68),*(int *)(param_1 + 0x74));
    }
    return;
}

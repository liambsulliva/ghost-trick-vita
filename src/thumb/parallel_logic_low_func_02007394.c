extern char data_020b18d8[];
extern char data_020b18dc[];
extern void func_0200743c(char *param);

void func_02007394(int param_1, int param_2, int param_3)
{
    char *block;
    int tag;

    tag = 0x13;
    block = data_020b18d8;
    *(int *)(block + 4) = tag;
    *(int *)(block + 8) = param_1;
    *(int *)(block + 0xc) = param_2;
    *(int *)(block + 0x10) = param_3;
    func_0200743c(data_020b18dc);
}

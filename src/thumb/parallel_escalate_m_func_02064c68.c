extern int data_020b0318;
extern int func_02031254(int size);
extern void func_0206448c(unsigned int *p);
extern int func_02064c54(int p, int kind);

unsigned int *func_02064c68(unsigned int *param_1)
{
    unsigned int i;
    int p;

    func_0206448c(param_1);
    *param_1 = (unsigned int)&data_020b0318;
    param_1[0x20] = 0;
    param_1[0x21] = 0;
    *(unsigned char *)(param_1 + 0x26) = 0;
    *(unsigned char *)((int)param_1 + 0x99) = 0;
    *(unsigned char *)((int)param_1 + 0x9a) = 0;
    *(unsigned char *)((int)param_1 + 0x9b) = 0;
    *(unsigned char *)(param_1 + 0x27) = 0;
    param_1[0x1e] = 0;
    param_1[0x1f] = 0;
    for (i = 0; i < 4; i++) {
        param_1[i + 0xe] = 0;
        param_1[i + 0x12] = 0;
        param_1[i + 0x16] = 0;
        param_1[i + 0x1a] = 0;
    }
    for (i = 0; i < 2; i++) {
        param_1[i + 0x22] = 0;
        param_1[i + 0x24] = 0;
    }
    p = func_02031254(0x14);
    param_1[0x1e] = (unsigned int)(p ? func_02064c54(p, 0) : p);
    p = func_02031254(0x14);
    param_1[0x1f] = (unsigned int)(p ? func_02064c54(p, 1) : p);
    return param_1;
}

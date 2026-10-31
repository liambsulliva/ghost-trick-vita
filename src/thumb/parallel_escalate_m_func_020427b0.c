extern void func_02042820(unsigned char *self, unsigned char *data, int size, int p3, unsigned char a, int b, unsigned short c, int e);
extern void func_020428f0(unsigned char *self, unsigned char *data, int size, int p3, int p4, unsigned short c, int d, int f, int g);

void func_020427b0(unsigned char *param_1, unsigned char *param_2, int param_3, int param_4, unsigned char param_5, int param_6, unsigned short param_7, int param_8, int param_9, int param_10, int param_11)
{
    *(unsigned char **)(param_1 + 0x3c) = param_2;
    *(int *)(param_1 + 0x18) = param_2[9];
    *(int *)(param_1 + 0x1c) = param_2[10];
    *(int *)(param_1 + 0x20) = param_2[8];
    *(int *)(param_1 + 0x28) = *(int *)(param_2 + 0x1c);
    *(int *)(param_1 + 0x2c) = *(int *)(param_2 + 0x20);
    *(int *)(param_1 + 0x30) = param_11;
    *(int *)(param_1 + 0x34) = param_6;
    func_02042820(param_1, param_2 + *(int *)(param_2 + 0xc), *(int *)(param_2 + 0x10), param_3, param_5, param_6, param_7, param_9);
    func_020428f0(param_1, param_2 + *(int *)(param_2 + 0x14), *(int *)(param_2 + 0x18), param_3, param_4, param_7, param_8, param_10, param_11);
}

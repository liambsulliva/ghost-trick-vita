extern unsigned int *func_02008444(void);
extern unsigned int func_0203dcb4(char *param_1);
extern char *func_0201230c(char *param_1);
extern void func_020083b0(int *param_1, int *param_2);
extern unsigned int func_ov006_020d3ac4(int param_1);

void func_ov006_020d3a40(char *param_1)
{
    unsigned int i;
    unsigned char k;

    *(short *)(param_1 + 0x30) = (short)func_0203dcb4((char *)func_02008444());
    func_020083b0((int *)(param_1 + 0x34), (int *)func_0201230c((char *)func_02008444()));
    *(int *)(param_1 + 0x40) = (int)func_ov006_020d3ac4((int)func_02008444());
    *(int *)(param_1 + 0x4c) = 0;
    *(int *)(param_1 + 0x50) = 0;
    *(int *)(param_1 + 0x54) = 0;
    *(int *)(param_1 + 0x44) = 0;
    *(int *)(param_1 + 0x48) = 0;
    *(int *)(param_1 + 0x58) = 0;
    *(int *)(param_1 + 0x5c) = 0;
    *(int *)(param_1 + 0x60) = 0;
    *(int *)(param_1 + 0x64) = 0;
    i = 0;
    do {
        *(int *)(param_1 + i * 4 + 0x68) = 0;
        i++;
    } while (i < 2);
    k = 0;
    do {
        *(int *)(param_1 + k * 4 + 0x70) = 0;
        k++;
    } while (k < 9);
    *(int *)(param_1 + 0x94) = 0;
    if (*(int *)(param_1 + 0x20) == 0) {
        *(unsigned char *)(param_1 + 0x28) = 0;
    } else {
        *(unsigned char *)(param_1 + 0x28) = 2;
    }
    *(unsigned char *)(param_1 + 0x27) = 0;
}

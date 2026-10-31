extern char data_020b9df4;
extern void func_02016fbc(int a, int b);
extern void func_02016fc4(int a, unsigned short b);
extern void func_02017714(int *a, int b);
extern void func_0201778c(int *a);
extern void func_0201c634(int a, unsigned int b, int c, int d, int e);
extern void func_0201c69c(int a, int b, int c, int d, int e);
extern void func_0201c6c4(int a, int b);
extern void func_0201cf2c(int a, int b);

void func_02016ef8(int *param_1, int param_2, unsigned int param_3)
{
    int uVar1;

    *(unsigned short *)(param_1 + 8) = 6;
    func_0201778c(param_1);
    *(unsigned short *)((int)param_1 + 0x2a) = 3;
    param_1[6] = param_2;
    param_1[5] = param_2;
    func_02017714(param_1, 0);
    *(unsigned short *)((int)param_1 + 0x42) = 0x30;
    *(unsigned short *)(param_1 + 0x11) = 8;
    *(unsigned short *)((int)param_1 + 0x46) = 0x100;
    *(unsigned short *)(param_1 + 0x12) = 0x30;
    *(unsigned short *)(param_1 + 0x13) = *(unsigned short *)((int)param_1 + 0x46);
    *(unsigned short *)((int)param_1 + 0x4a) = *(unsigned short *)(param_1 + 0x13);
    uVar1 = (int)&data_020b9df4;
    func_0201c6c4(uVar1, param_1[4]);
    func_0201cf2c(uVar1, param_1[0x18]);
    func_02016fbc(uVar1, 3);
    func_0201c634(uVar1, param_3, *(unsigned short *)((int)param_1 + 0x46), *(unsigned short *)(param_1 + 0x12), 2);
    func_02016fc4(uVar1, (unsigned short)(*(unsigned short *)((int)param_1 + 0x46) - 0x10));
    func_0201c69c(uVar1, param_1[5],
                  (short)(*(short *)((int)param_1 + 0x42) + *(short *)(param_1 + 0xf)),
                  *(short *)(param_1 + 0x11), *(unsigned short *)((int)param_1 + 0x2a) & 0xff);
}

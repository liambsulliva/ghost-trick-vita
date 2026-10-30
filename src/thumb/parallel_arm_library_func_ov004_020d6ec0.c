extern void func_02023bd4(int);
extern unsigned int *func_02008444(void);
extern void func_ov004_020d6c8c(unsigned int *, unsigned int);
extern void func_0202fbb0(int *);
extern int func_0200eac4(int *);
extern int func_0200eab4(int *);
extern int func_02008828(int, int);
extern int func_0200eab8(int *);
extern void func_0200eabc(int *, int);
extern void func_020599c0(void);
extern unsigned char data_020cff60[];
extern unsigned int data_020c9e48[];
extern unsigned char data_020cf8b4[];

void func_ov004_020d6ec0(int param_1)
{
    int stack[4];
    int bits;

    func_02023bd4(0x020e4614);
    func_ov004_020d6c8c(func_02008444(), 0);
    bits = data_020cff60[0x15];
    bits &= ~0x40;
    data_020cff60[0x15] = bits;
    data_020c9e48[0] = *(unsigned int *)(param_1 + 0x20);
    func_0202fbb0(stack);
    while (func_0200eac4(stack) != 0) {
        if (func_02008828(func_0200eab4(stack), (int)data_020cf8b4) != 0) {
            func_0200eab8(stack);
            func_020599c0();
        }
        func_0200eabc(stack, 0);
    }
}

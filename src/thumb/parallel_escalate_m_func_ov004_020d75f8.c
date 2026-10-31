extern unsigned char data_ov004_020e4750[];
extern unsigned short data_ov004_020e4808[];
extern void *func_0202f25c(unsigned char *object);
extern void func_ov004_020d7050(void *a, void *b, int c);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *a, int b);
extern void func_0202a080(void *a, unsigned short b, unsigned short c);

void func_ov004_020d75f8(unsigned char *param_1)
{
    func_ov004_020d7050(func_0202f25c(param_1), data_ov004_020e4750, -1);
    func_0202a0c0(func_0200847c(), 0x1eb);
    param_1[0x25] = 1;
    func_0202a080(func_0200847c(), data_ov004_020e4808[1], data_ov004_020e4808[0]);
}

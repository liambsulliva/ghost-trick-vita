extern unsigned char data_020c9e88[];
extern unsigned char data_020cdd90[];
extern unsigned char data_020cf75c[];
int func_ov004_020d707c(int);
extern void *func_02009900(void);
extern void func_0202fad4(void *a, void *b);
extern void func_02027528(int a);

void func_ov004_020d7234(void)
{
    func_ov004_020d707c(-16);
    *(short *)(data_020c9e88 + 0x12) = -16;
    func_0202fad4(func_02009900(), data_020cdd90);
    func_0202fad4(func_02009900(), data_020cf75c);
    func_02027528(0x17f4);
}

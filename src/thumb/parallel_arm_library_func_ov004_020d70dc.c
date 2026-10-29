extern unsigned char data_020c9e88[];
extern unsigned int *func_02008444(void);
void func_02031b30(unsigned char *, int);
void func_02027528(int);
void func_ov004_020d707c(int);

void func_ov004_020d70dc(void)
{
    func_ov004_020d707c(0x10);
    *(unsigned short *)(data_020c9e88 + 0x12) = 0x10;
    func_02031b30((unsigned char *)func_02008444(), 0);
    func_02027528(0x17f1);
}

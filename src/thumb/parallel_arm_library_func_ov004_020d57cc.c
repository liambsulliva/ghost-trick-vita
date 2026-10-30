extern void func_0207193c(int);
extern void func_02071dbc(void);
extern void func_02071220(unsigned int);
extern void func_02071424(unsigned int);
extern void func_0207303c(unsigned char *, unsigned char *, int);
extern unsigned char *func_02008860(void);
extern void func_0200c82c(int, int);
extern void func_0203f5b4(int);
extern unsigned char *func_0202f25c(void);
extern int func_ov004_020d57b0(int, int);
extern unsigned char data_020c9e88[];
extern unsigned char data_020c9e48[];

void func_ov004_020d57cc(void)
{
    short value;

    func_0207193c(3);
    func_ov004_020d57b0(0x6800000, 0x40000);
    func_02071dbc();
    func_02071220(*(unsigned int *)(func_0202f25c() + 0xb00));
    func_02071424(*(unsigned int *)(func_0202f25c() + 0xb04));
    value = *(short *)(data_020c9e88 + 0x12);
    func_0207303c(func_0202f25c() + 0xaac, data_020c9e48, 0x54);
    *(short *)(data_020c9e88 + 0x12) = value;
    func_0200c82c((int)func_02008860(), 0);
    func_0203f5b4((int)func_02008860());
}

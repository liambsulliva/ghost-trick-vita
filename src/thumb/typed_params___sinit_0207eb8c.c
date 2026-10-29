extern char data_020856fc;
extern char data_020a549c;
extern char data_020cb030;
extern char data_020cb04c;
extern int func_02008398();
extern unsigned char * func_0202ff84();

void __sinit_0207eb8c(void)

{
    func_0202ff84(((unsigned char *)&data_020cb04c),((int)&data_020856fc),((int)&data_020cb030),0xbc,0,0);
    func_02008398(((unsigned char *)&data_020a549c),0x80000,0x60000,0);
    return;
}

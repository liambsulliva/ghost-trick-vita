extern char data_0209da80;
extern char data_0209da8c;
extern char data_0209da9c;
extern char data_020cb030;
extern char data_020cf724;
extern char data_020cf980;
extern char data_020cf99c;
extern char data_020cf9b8;
extern unsigned char * func_0205bc0c();
extern unsigned char * func_0205bc28();
extern unsigned char * func_0205bc44();

void __sinit_020808c0(void)

{
    unsigned char *puVar1;

    func_0205bc0c(((unsigned char *)&data_020cf980),((int)&data_0209da80),((int)&data_020cb030),0x124,0,0);
    puVar1 = ((unsigned char *)&data_020cf9b8);
    func_0205bc28(((unsigned char *)&data_020cf9b8),((int)&data_0209da8c),((int)&data_020cf724),200,0,0);
    func_0205bc44(((unsigned char *)&data_020cf99c),((int)&data_0209da9c),(int)puVar1,0xcc,0,0);
    return;
}

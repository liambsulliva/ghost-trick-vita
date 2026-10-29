extern char data_0209b644;
extern char data_0209b65c;
extern char data_0209b66c;
extern char data_0209b680;
extern char data_020ce6b4;
extern char data_020ce6e8;
extern char data_020ce704;
extern char data_020ce720;
extern char data_020ce73c;
extern unsigned char * func_02046554();
extern unsigned char * func_02046570();
extern unsigned char * func_0204658c();
extern unsigned char * func_020465a8();

void __sinit_0207f94c(void)

{
    unsigned char *puVar1;

    puVar1 = ((unsigned char *)&data_020ce73c);
    func_02046554(((unsigned char *)&data_020ce73c),((int)&data_0209b65c),((int)&data_020ce6b4),0x130,0,0);
    func_02046570(((unsigned char *)&data_020ce720),((int)&data_0209b66c),(int)puVar1,0x144,0,0);
    func_0204658c(((unsigned char *)&data_020ce6e8),((int)&data_0209b680),(int)puVar1,0x134,0,0);
    func_020465a8(((unsigned char *)&data_020ce704),((int)&data_0209b644),(int)puVar1,0x17c,0,0);
    return;
}

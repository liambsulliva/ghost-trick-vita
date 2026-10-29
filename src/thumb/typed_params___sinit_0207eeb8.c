extern char data_0209a4b0;
extern char data_0209a4c0;
extern char data_020c9db0;
extern char data_020cda64;
extern char data_020cda80;
extern unsigned char * func_02035d54();
extern unsigned char * func_02035d70();

void __sinit_0207eeb8(void)

{
    unsigned char *puVar1;

    puVar1 = ((unsigned char *)&data_020cda64);
    func_02035d54(((unsigned char *)&data_020cda64),((int)&data_0209a4b0),((int)&data_020c9db0),0x19c,0,0);
    func_02035d70(((unsigned char *)&data_020cda80),((int)&data_0209a4c0),(int)puVar1,0x19c,0,0);
    return;
}

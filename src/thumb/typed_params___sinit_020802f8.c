extern char data_0209d5fc;
extern char data_0209d610;
extern char data_0209d620;
extern char data_020cf110;
extern char data_020cf2cc;
extern char data_020cf2e8;
extern char data_020cf304;
extern unsigned char * func_020539c8();
extern unsigned char * func_020539e4();
extern unsigned char * func_02053a00();

void __sinit_020802f8(void)

{
    unsigned char *puVar1;

    puVar1 = ((unsigned char *)&data_020cf2e8);
    func_020539c8(((unsigned char *)&data_020cf2e8),((int)&data_0209d5fc),((int)&data_020cf110),0x15c,0,0);
    func_020539e4(((unsigned char *)&data_020cf2cc),((int)&data_0209d610),(int)puVar1,0x15c,0,0);
    func_02053a00(((unsigned char *)&data_020cf304),((int)&data_0209d620),(int)puVar1,0x15c,0,0);
    return;
}

extern char data_020a46a4;
extern char data_020a46b0;
extern char data_020a46bc;
extern char data_020a46c8;
extern char data_020a46d4;
extern int func_02008398();

void __sinit_0207e70c(void)

{
    func_02008398(((unsigned char *)&data_020a46c8),0,0,0);
    func_02008398(((unsigned char *)&data_020a46bc),0x1000,0x1000,0x1000);
    func_02008398(((unsigned char *)&data_020a46b0),0x1000,0,0);
    func_02008398(((unsigned char *)&data_020a46a4),0,0x1000,0);
    func_02008398(((unsigned char *)&data_020a46d4),0,0,0x1000);
    return;
}

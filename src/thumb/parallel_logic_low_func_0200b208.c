extern void func_0200b240();
extern char data_020b8448;
extern char data_020b844c;
extern char data_020b84b8;
extern unsigned char * func_02023db8();
extern int func_0207da20();

unsigned char * func_0200b208(void)

{
    unsigned int *puVar1;
    unsigned char *puVar2;

    puVar1 = ((unsigned int *)&data_020b8448);
    if ((*((unsigned int *)&data_020b8448) & 1) == 0) {
        puVar2 = ((unsigned char *)&data_020b84b8);
        func_02023db8(((unsigned char *)&data_020b84b8));
        func_0207da20((int)puVar2,((int)((unsigned int)func_0200b240)),((unsigned char *)&data_020b844c));
        *puVar1 = *puVar1 | 1;
    }
    return ((unsigned char *)&data_020b84b8);
}

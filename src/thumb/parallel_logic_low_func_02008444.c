extern char data_020b191c;
extern char data_020b192c;
extern char data_020b1be0;
extern unsigned int * func_0200e64c();
extern int func_0207da20();

unsigned int * func_02008444(void)

{
    unsigned int *puVar1;
    unsigned int *puVar2;

    puVar1 = ((unsigned int *)&data_020b191c);
    if ((*((unsigned int *)&data_020b191c) & 1) == 0) {
        puVar2 = ((unsigned int *)&data_020b1be0);
        func_0200e64c(((unsigned int *)&data_020b1be0));
        func_0207da20((int)puVar2,((int)0x0200e791u),((unsigned char *)&data_020b192c));
        *puVar1 = *puVar1 | 1;
    }
    return ((unsigned int *)&data_020b1be0);
}

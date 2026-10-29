extern void func_02008898();
extern char data_020b1914;
extern char data_020b1938;
extern char data_020b2cc8;
extern unsigned char * func_0200c534();
extern int func_0207da20();

unsigned char * func_02008860(void)

{
    unsigned int *puVar1;
    unsigned char *puVar2;

    puVar1 = ((unsigned int *)&data_020b1914);
    if ((*((unsigned int *)&data_020b1914) & 1) == 0) {
        puVar2 = ((unsigned char *)&data_020b2cc8);
        func_0200c534(((unsigned char *)&data_020b2cc8));
        func_0207da20((int)puVar2,((int)((unsigned int)func_02008898)),((unsigned char *)&data_020b1938));
        *puVar1 = *puVar1 | 1;
    }
    return ((unsigned char *)&data_020b2cc8);
}

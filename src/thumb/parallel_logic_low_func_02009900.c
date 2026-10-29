extern void func_02009938();
extern char data_020b42a0;
extern char data_020b42b0;
extern char data_020b42f4;
extern unsigned char * func_0202f76c();
extern int func_0207da20();

unsigned char * func_02009900(void)

{
    unsigned int *puVar1;
    unsigned char *puVar2;

    puVar1 = ((unsigned int *)&data_020b42a0);
    if ((*((unsigned int *)&data_020b42a0) & 1) == 0) {
        puVar2 = ((unsigned char *)&data_020b42f4);
        func_0202f76c(((unsigned char *)&data_020b42f4));
        func_0207da20((int)puVar2,((int)((unsigned int)func_02009938)),((unsigned char *)&data_020b42b0));
        *puVar1 = *puVar1 | 1;
    }
    return ((unsigned char *)&data_020b42f4);
}

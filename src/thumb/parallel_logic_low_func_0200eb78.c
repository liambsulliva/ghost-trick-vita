extern void func_0200ebb0();
extern char data_020b89a8;
extern char data_020b89b0;
extern char data_020b8b3c;
extern unsigned short * func_02022a10();
extern int func_0207da20();

unsigned short * func_0200eb78(void)

{
    unsigned int *puVar1;
    unsigned short *puVar2;

    puVar1 = ((unsigned int *)&data_020b89a8);
    if ((*((unsigned int *)&data_020b89a8) & 1) == 0) {
        puVar2 = ((unsigned short *)&data_020b8b3c);
        func_02022a10(((unsigned short *)&data_020b8b3c));
        func_0207da20((int)puVar2,((int)((unsigned int)func_0200ebb0)),((unsigned char *)&data_020b89b0));
        *puVar1 = *puVar1 | 1;
    }
    return ((unsigned short *)&data_020b8b3c);
}

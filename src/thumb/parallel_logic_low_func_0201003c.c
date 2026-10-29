extern void func_0201587c();
extern char data_020b89a4;
extern char data_020b89c8;
extern char data_020b8a80;
extern unsigned int * func_02015864();
extern int func_0207da20();

unsigned int * func_0201003c(void)

{
    unsigned int *puVar1;
    unsigned int *puVar2;

    puVar1 = ((unsigned int *)&data_020b89a4);
    if ((*((unsigned int *)&data_020b89a4) & 1) == 0) {
        puVar2 = ((unsigned int *)&data_020b8a80);
        func_02015864(((unsigned int *)&data_020b8a80));
        func_0207da20((int)puVar2,((int)((unsigned int)func_0201587c)),((unsigned char *)&data_020b89c8));
        *puVar1 = *puVar1 | 1;
    }
    return ((unsigned int *)&data_020b8a80);
}

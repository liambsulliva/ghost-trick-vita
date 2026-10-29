extern void func_02008bb8();
extern char data_020cc1b4;
extern char data_020cc1dc;
extern char data_020cc218;
extern unsigned char * func_02008b88();
extern int func_0207da20();

unsigned char * func_020312e4(void)

{
    unsigned int *puVar1;
    unsigned char *puVar2;

    puVar1 = ((unsigned int *)&data_020cc1b4);
    if ((*((unsigned int *)&data_020cc1b4) & 1) == 0) {
        puVar2 = ((unsigned char *)&data_020cc218);
        func_02008b88(((unsigned char *)&data_020cc218));
        func_0207da20((int)puVar2,((int)((unsigned int)func_02008bb8)),((unsigned char *)&data_020cc1dc));
        *puVar1 = *puVar1 | 1;
    }
    return ((unsigned char *)&data_020cc218);
}

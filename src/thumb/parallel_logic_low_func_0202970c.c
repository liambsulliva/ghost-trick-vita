extern void func_0200ad48();
extern char data_020ca0bc;
extern char data_020ca0c0;
extern char data_020ca0e4;
extern int func_0200ad30();
extern int func_0207da20();

unsigned char * func_0202970c(void)

{
    unsigned int *puVar1;
    unsigned char *puVar2;

    puVar1 = ((unsigned int *)&data_020ca0bc);
    if ((*((unsigned int *)&data_020ca0bc) & 1) == 0) {
        puVar2 = ((unsigned char *)&data_020ca0c0);
        func_0200ad30(((unsigned char *)&data_020ca0c0));
        func_0207da20((int)puVar2,((int)((unsigned int)func_0200ad48)),((unsigned char *)&data_020ca0e4));
        *puVar1 = *puVar1 | 1;
    }
    return ((unsigned char *)&data_020ca0c0);
}

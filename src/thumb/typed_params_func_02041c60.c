extern char data_0209b440;
extern char data_020c9d40;
extern unsigned char * func_0201123c();
extern unsigned char * func_02011264();
extern int func_02022f34();

void func_02041c60(void)

{
    unsigned char *puVar1;

    if ((*(unsigned short *)(((int)&data_020c9d40) + 2) & 2) != 0) {
        puVar1 = func_02011264();
        puVar1 = func_0201123c(puVar1,0);
        func_02022f34((int)puVar1,((int)&data_0209b440),0);
    }
    return;
}

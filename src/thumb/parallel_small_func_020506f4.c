extern char data_020ced8c;
extern unsigned char * func_02009900();
extern int func_0202f7e8();
extern unsigned char * func_020505c4();
extern int func_02050734();
extern int func_0205073c();

unsigned char * func_020506f4(int param_1,int param_2,unsigned int param_3)

{
    unsigned char *puVar1;
    unsigned char *puVar2;

    puVar1 = func_020505c4(&data_020ced8c);
    if (puVar1 != (unsigned char *)0x0) {
        func_02050734(puVar1,param_1);
        *(unsigned int *)(puVar1 + 0xc4) = param_3;
        if (param_2 != 0) {
            func_0205073c(puVar1);
        }
        puVar2 = func_02009900();
        func_0202f7e8((int)puVar2,(int)puVar1,7,0);
    }
    return puVar1;
}

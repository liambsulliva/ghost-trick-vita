extern char data_020a46bc;
extern char data_020a46c8;
extern char data_020a54f4;
extern int func_020082d8();
extern unsigned char * func_0202f524();

unsigned int * func_0202ffb4(unsigned int *param_1)

{
    unsigned int *puVar1;

    func_0202f524((unsigned char *)param_1);
    puVar1 = ((unsigned int *)&data_020a46c8);
    *param_1 = ((unsigned int)&data_020a54f4);
    func_020082d8(param_1 + 10,puVar1);
    func_020082d8(param_1 + 0xd,((unsigned int *)&data_020a46bc));
    *(unsigned short *)(param_1 + 0x10) = 0;
    return param_1;
}

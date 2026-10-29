extern char data_020a549c;
extern char data_020a54b0;
extern int func_020082d8();
extern unsigned char * func_0200b460();
extern unsigned char * func_0202f524();

unsigned int * func_0202fe60(unsigned int *param_1)

{
    func_0202f524((unsigned char *)param_1);
    *param_1 = ((unsigned int)&data_020a54b0);
    func_0200b460((unsigned char *)(param_1 + 10));
    func_020082d8(param_1 + 0x2c,((unsigned int *)&data_020a549c));
    return param_1;
}

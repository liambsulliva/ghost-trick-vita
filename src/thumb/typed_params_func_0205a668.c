extern char data_0209da40;
extern char data_0209da44;
extern char data_020aea68;
extern unsigned char * func_0202f524();

unsigned int * func_0205a668(unsigned int *param_1)

{
    func_0202f524((unsigned char *)param_1);
    *param_1 = ((unsigned int)&data_020aea68);
    param_1[0xf] = ((unsigned int)&data_0209da44);
    param_1[0x10] = ((unsigned int)&data_0209da40);
    *(unsigned char *)(param_1 + 0x11) = 0;
    return param_1;
}

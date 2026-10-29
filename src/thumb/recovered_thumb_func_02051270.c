extern char data_0209c108;
extern char data_020abd84;
extern int func_0200dac8();
extern int func_0202f524();
extern int func_02050e04();
extern int func_0205125c();
extern int func_02051264();
extern int func_02051648();

unsigned int * func_02051270(unsigned int *param_1)

{
    func_0202f524();
    *param_1 = ((unsigned int)&data_020abd84);
    func_02051648(param_1 + 10);
    func_02050e04(param_1 + 0x18);
    param_1[0x2d] = 0;
    param_1[0x3e] = 0;
    param_1[0x3f] = ((unsigned int)&data_0209c108);
    *(unsigned char *)(param_1 + 0x40) = 0;
    func_0205125c(param_1 + 10,param_1,param_1 + 0x18);
    func_02051264(param_1 + 0x2e,0x40);
    func_0200dac8(param_1);
    return param_1;
}

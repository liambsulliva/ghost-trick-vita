extern char data_02083cc8;
extern char data_020a2f08;
extern int func_0200dfe4();
extern int func_0200e0b4();
extern int func_0200e104();
extern int func_0201ef58();

unsigned int * func_0200dcd0(unsigned int *param_1,unsigned int param_2,unsigned int param_3)

{
    int iVar1;

    *param_1 = ((unsigned int)&data_020a2f08);
    param_1[1] = 4;
    *(unsigned char *)(param_1 + 2) = 3;
    param_1[5] = ((unsigned int)&data_02083cc8);
    *(char *)((int)param_1 + 9) = (char)param_3;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
    func_0201ef58(param_1 + 10);
    func_0201ef58(param_1 + 0x1e);
    param_1[8] = 0;
    param_1[9] = 0;
    func_0200dfe4(param_1);
    iVar1 = func_0200e104(param_1,param_2);
    if (iVar1 != 0) {
        func_0200e0b4(param_1,param_3);
    }
    return param_1;
}

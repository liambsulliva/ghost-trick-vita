extern int func_0200dae8();
extern int func_0202f5e4();
extern int func_02050e44();
extern int func_020512d4();
extern int func_020514d8();

unsigned int * func_020512e0(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020abd84u);
    func_0200dae8((int)param_1);
    func_020514d8((int)param_1);
    func_02050e44((unsigned char *)(param_1 + 0x18));
    func_020512d4((unsigned char *)(param_1 + 10));
    func_0202f5e4((unsigned char *)param_1);
    return param_1;
}

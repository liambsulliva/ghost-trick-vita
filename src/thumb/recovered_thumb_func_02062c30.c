extern int func_02009900();
extern int func_020153d8();
extern int func_0202f7e8();
extern int func_02061cd8();
extern int func_020620b8();

unsigned int * func_02062c30(unsigned int *param_1)

{
    unsigned int uVar1;

    func_020620b8();
    *param_1 = ((unsigned int)0x020afbfcu);
    param_1[0x3d] = 0;
    uVar1 = func_02061cd8(((unsigned int)0x020cfc14u));
    param_1[0x3d] = uVar1;
    func_020153d8(uVar1,param_1);
    uVar1 = func_02009900();
    func_0202f7e8(uVar1,param_1[0x3d],7,0);
    return param_1;
}

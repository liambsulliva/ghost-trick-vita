typedef int code();
extern unsigned char * func_02009900();
extern int func_020153d8();
extern int func_0202f7e8();
extern unsigned int * func_02047120();
extern int func_0204c2fc();
extern int func_02061d14();

unsigned int * func_0204c304(unsigned int *param_1)

{
    unsigned int uVar1;
    unsigned char *puVar2;

    func_02047120(param_1);
    *param_1 = ((unsigned int)0x020ab37cu);
    param_1[0x2e] = 0;
    uVar1 = func_02061d14(((unsigned int)0x020cfc4cu));
    param_1[0x2e] = uVar1;
    puVar2 = func_02009900();
    func_0202f7e8((int)puVar2,param_1[0x2e],7,0);
    (**(code **)(*(int *)param_1[0x2e] + 0x84))((int *)param_1[0x2e],0x620);
    func_0204c2fc((unsigned char *)param_1[0x2e],0x600);
    func_020153d8(param_1[0x2e],(unsigned char *)param_1);
    param_1[0x2f] = ((unsigned int)0x00000a29u);
    param_1[0x30] = 0x3d;
    return param_1;
}

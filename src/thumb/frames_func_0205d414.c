extern int func_020083b0();
extern int func_02058a80();
extern int func_0205d534();
extern unsigned char * func_0205d760();

void func_0205d414(unsigned int param_1,int param_2,unsigned int param_3)

{
    unsigned int *puVar1;
    unsigned char *puVar2;
    unsigned int auStack_1c [3];
    unsigned int auStack_28 [3];

    func_0205d534(param_1);
    *(int *)(param_1 + 0x94) = param_2;
    *(unsigned int *)(param_1 + 0x98) = param_3;
    func_02058a80((unsigned char *)auStack_1c,param_2);
    puVar1 = (unsigned int *)func_020083b0((unsigned int *)(param_1 + 0xa4),auStack_1c);
    func_020083b0((unsigned int *)(param_1 + 0xd4),puVar1);
    func_02058a80((unsigned char *)auStack_28,*(int *)(param_1 + 0x98));
    puVar1 = (unsigned int *)func_020083b0((unsigned int *)(param_1 + 0xe4),auStack_28);
    func_020083b0((unsigned int *)(param_1 + 0x114),puVar1);
    puVar2 = func_0205d760(param_1,param_1 + 0xd4);
    *(unsigned char **)(param_1 + 0x9c) = puVar2;
    puVar2 = func_0205d760(param_1,param_1 + 0x114);
    *(unsigned char **)(param_1 + 0xa0) = puVar2;
    return;
}

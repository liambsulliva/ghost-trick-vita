extern int func_020082d8();
extern int func_02008378();
extern unsigned char * func_020083a0();
extern int func_020083b0();
extern int func_02056cb8();
extern int func_0205958c();

void func_0206241c(int param_1)
{
    unsigned int *puVar1;
    unsigned int auStack_20 [3];
    unsigned int auStack_2c [3];
    unsigned int auStack_38 [3];

    func_02056cb8(param_1);
    if ((*(int *)(param_1 + 0x7c) != 0) && (*(short *)(param_1 + 0x96) >= 0)) {
        func_0205958c(auStack_20,*(int *)(param_1 + 0x7c),(int)*(short *)(param_1 + 0x96));
        puVar1 = (unsigned int *)func_020083a0(*(unsigned char **)(param_1 + 0x7c));
        func_020082d8(auStack_2c,puVar1);
        func_02008378((unsigned char *)auStack_38,(unsigned char *)auStack_2c,(unsigned char *)auStack_20);
        func_020083b0((unsigned int *)(param_1 + 0x28),auStack_38);
    }
    return;
}

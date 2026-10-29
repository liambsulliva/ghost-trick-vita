extern int func_020082d8();
extern int func_02008378();
extern unsigned char * func_020083a0();
extern int func_020083b0();
extern int func_0205958c();

void func_02047250(unsigned int *param_1,int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int *puVar1;
    unsigned int uVar2;
    unsigned int auStack_24 [3];
    unsigned int auStack_30 [3];
    unsigned int auStack_3c [3];
    unsigned char auStack_48 [12];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020082d8(param_1,(unsigned int *)(param_2 + 0x28));
    if (*(int *)(param_2 + 0xac) != 0) {
        uVar2 = (unsigned int)*(short *)(param_2 + 0x82);
        if ((int)uVar2 < 0) {
            uVar2 = 0;
        }
        func_0205958c(auStack_24,*(int *)(param_2 + 0xac),uVar2);
        puVar1 = (unsigned int *)func_020083a0(*(unsigned char **)(param_2 + 0xac));
        func_020082d8(auStack_30,puVar1);
        func_02008378(auStack_48,(unsigned char *)auStack_30,(unsigned char *)auStack_24);
        func_02008378((unsigned char *)auStack_3c,auStack_48,(unsigned char *)(param_2 + 0x28));
        func_020083b0(param_1,auStack_3c);
    }
    return;
}

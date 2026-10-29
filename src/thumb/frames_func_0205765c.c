extern int func_020576bc();
extern int func_020576c8();

void func_0205765c(int param_1,int param_2,int param_3,int param_4,int param_5)

{
    unsigned int auStack_20 [2];
    unsigned int auStack_28 [2];
    unsigned int auStack_30 [2];
    unsigned int auStack_38 [2];
    int iStack_18;

    iStack_18 = param_4;
    func_020576c8((unsigned char *)auStack_20,param_2,param_3);
    func_020576bc((unsigned int *)(param_1 + 0xa4),auStack_20);
    func_020576c8((unsigned char *)auStack_28,param_2,param_5);
    func_020576bc((unsigned int *)(param_1 + 0xac),auStack_28);
    func_020576c8((unsigned char *)auStack_30,param_4,param_5);
    func_020576bc((unsigned int *)(param_1 + 0xb4),auStack_30);
    func_020576c8((unsigned char *)auStack_38,param_4,param_3);
    func_020576bc((unsigned int *)(param_1 + 0xbc),auStack_38);
    return;
}

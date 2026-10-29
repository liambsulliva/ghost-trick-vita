extern char data_02083e2c;
extern char data_02083e34;
extern int func_02012400();

void func_02011094(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    if (*(int *)(param_1 + ((int)0x00000e58u)) != 0x19) {
        *(unsigned int *)(param_1 + ((int)0x00000e58u)) = 3;
        func_02012400(param_1,((int)&data_02083e2c),((int)&data_02083e34),param_4);
    }
    return;
}

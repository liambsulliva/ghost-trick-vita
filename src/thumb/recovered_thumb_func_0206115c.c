extern int func_020266dc();
extern int func_0205eb18();
extern int func_02060d54();
extern int func_02060f54();

void func_0206115c(int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;

    uVar1 = func_020266dc(param_2,param_3);
    *(unsigned int *)(param_1 + 0xfc) = uVar1;
    func_02060d54(param_1,0);
    func_02060f54(param_1,1);
    func_0205eb18(*(unsigned int *)(param_1 + 0xdc));
    return;
}

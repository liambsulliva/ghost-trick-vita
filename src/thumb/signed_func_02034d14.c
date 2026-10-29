extern int func_02029708();
extern unsigned char * func_0202970c();
extern int func_020501f4();

void func_02034d14(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned char *puVar1;
    int iVar2;

    puVar1 = func_0202970c();
    iVar2 = func_02029708(puVar1);
    func_020501f4(*(int *)(param_1 + 0x28),*(unsigned int *)(param_1 + 0x2c),
                                iVar2 - (unsigned int)*(unsigned short *)(param_1 + 0x42),*(int *)(param_1 + 0x34),0x20,0x3e0,
                                (int)*(signed char *)(param_1 + 0x44),(int)*(signed char *)(param_1 + 0x45),0);
    return;
}

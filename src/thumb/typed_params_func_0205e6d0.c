extern int func_02029708();
extern unsigned char * func_0202970c();
extern int func_020501f4();
extern int func_020570e4();

void func_0205e6d0(int *param_1)

{
    unsigned char *puVar1;
    int iVar2;

    if (*(unsigned char *)(param_1 + 0x34) < 3) {
        func_020570e4(param_1);
    }
    else {
        puVar1 = func_0202970c();
        iVar2 = func_02029708(puVar1);
        func_020501f4(0,0,iVar2 + -0x8b0,param_1[0x35],0x52,0x1f,(param_1[0x38] << 4) >> 0x10,0x3c,0);
    }
    return;
}

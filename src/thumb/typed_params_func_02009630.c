extern int func_01ff80b8();
extern int func_02008ea4();
extern unsigned char * func_02008eb0();

void func_02009630(unsigned int param_1,unsigned int param_2,unsigned char *param_3)

{
    unsigned int uVar1;
    unsigned int *puVar2;
    unsigned int uVar3;

    func_02008ea4(param_3,0x20);
    uVar1 = ((unsigned int)0x73646174u);
    *(unsigned int *)(param_3 + 4) = 0;
    *(unsigned int *)(param_3 + 8) = uVar1;
    param_3[0x10] = 0;
    param_3[0x11] = 0xd;
    puVar2 = (unsigned int *)func_02008eb0();
    uVar3 = func_01ff80b8(puVar2);
    *(unsigned int *)(param_3 + 0xc) = uVar3;
    return;
}

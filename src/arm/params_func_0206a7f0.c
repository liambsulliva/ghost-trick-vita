extern char data_0206a270;
extern char data_020d02a0;
extern unsigned char func_02069eb4();
extern int func_0206a7a0();

unsigned char func_0206a7f0(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4,
                                    unsigned int param_5,int param_6,unsigned int param_7,unsigned int param_8,int param_9)

{
    int iVar1;
    unsigned char *puVar2;
    unsigned char bVar3;
    unsigned int uVar4;

    uVar4 = 1;
    if (param_9 != 0) {
        uVar4 = 2;
    }
    func_0206a7a0(uVar4,param_4,param_5);
    iVar1 = ((int)&data_020d02a0);
    *(unsigned int *)(((int)&data_020d02a0) + 0x4fc) = param_1;
    *(unsigned int *)(iVar1 + 0x500) = param_2;
    *(unsigned int *)(iVar1 + 0x504) = param_3;
    *(unsigned int *)(iVar1 + 0x510) = param_7;
    *(unsigned int *)(iVar1 + 0x514) = param_8;
    puVar2 = ((unsigned char *)&data_0206a270);
    *(int *)(iVar1 + 0x518) = param_9;
    bVar3 = func_02069eb4(puVar2,param_6,iVar1,param_9);
    return bVar3;
}

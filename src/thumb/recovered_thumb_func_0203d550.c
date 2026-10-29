extern char data_0209ab04;
extern int func_02008444();
extern int func_02010384();
extern int func_0203cd24();
extern int func_0203cd64();
extern int func_0203cdec();
extern int func_020521d4();
extern int func_02057994();

void func_0203d550(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((int)0x00000142u);
    uVar2 = func_02057994(((int)0x00000142u),0x6f,0,0x34000,1);
    *(unsigned int *)(param_1 + 0x28) = uVar2;
    uVar2 = func_02057994(iVar1 + 0x5a,0x6f,0,((unsigned int)0xfffb4000u),1);
    *(unsigned int *)(param_1 + 0x2c) = uVar2;
    uVar2 = func_02057994(0x151,0x6f,0,0x14000,1);
    *(unsigned int *)(param_1 + 0x30) = uVar2;
    func_0203cdec(param_1);
    func_0203cd64(param_1,0,1);
    func_0203cd24(param_1);
    uVar2 = ((unsigned int)&data_0209ab04);
    *(unsigned char *)(param_1 + 4) = 0;
    func_020521d4(uVar2,0,6,0x100);
    uVar2 = func_02008444();
    func_02010384(uVar2,1);
    return;
}

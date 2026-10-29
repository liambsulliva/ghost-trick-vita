extern int func_02007b70();
extern int func_02007bd8();
extern int func_0201e250();
extern int func_0201f19c();
extern int func_0201f214();
extern int func_020314c8();
extern int func_020314d8();
extern int func_020314e4();

void func_02031460(void)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    int iVar4;

    func_02007bd8();
    uVar1 = func_020314c8(0x20000,0x20);
    uVar2 = func_02007bd8();
    func_0201f19c(uVar2,uVar1,0x20000);
    uVar3 = func_020314d8();
    iVar4 = func_020314e4();
    iVar4 = (uVar3 & 0xfffffff0) - (iVar4 + 0xfU & 0xfffffff0);
    uVar1 = func_020314c8(iVar4,4);
    uVar2 = func_02007bd8();
    func_0201f214(uVar2,uVar1,iVar4);
    func_02007bd8();
    iVar4 = func_02007b70();
    if (iVar4 != 0) {
        func_0201e250(iVar4,((unsigned int)0xfffc8000u),0xc00);
    }
    return;
}

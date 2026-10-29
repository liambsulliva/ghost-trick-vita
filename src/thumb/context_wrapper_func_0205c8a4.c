extern int func_02008444();
extern int func_0200eb78();
extern int func_02010b54();
extern int func_02022ca4();
extern int func_02029640();
extern int func_02029708();
extern int func_0202970c();
extern int func_0203435c();
extern int func_0205c9a0();
extern int func_0205c9a8();
extern int func_0205c9b0();
extern int func_0205c9bc();
extern int func_0205c9f8();
extern int func_0205ca1c();
extern int func_0205ca28();
extern int func_0205ca34();
extern int func_0205ca50();
extern int func_0205c9b4();
extern int func_0206e9d4();
extern int func_0206f77c();
extern int func_0207b694();

void func_0205c8a4(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    int iVar2;
    int iVar3;
    int iVar4;
    unsigned short *puVar5;
    unsigned char auStack_48 [48];
    unsigned int uStack_18;

    uStack_18 = param_4;
    uVar1 = func_0200eb78();
    uVar1 = func_02022ca4(uVar1,*(unsigned short *)(param_1 + 0x48));
    func_0202970c();
    iVar2 = func_02029708();
    func_02008444();
    iVar3 = func_0203435c();
    if (iVar3 == 0) {
        iVar3 = func_02029640(param_1);
        if (iVar3 != 0) {
            iVar4 = func_02010b54();
            if (iVar4 == 0) {
                return;
            }
            iVar4 = func_0205c9a0(iVar3);
            if (iVar4 == 0) {
                return;
            }
            puVar5 = (unsigned short *)func_0205c9b0(iVar3);
            func_0205c9a8(param_1,*puVar5);
        }
        func_0205c9b4(auStack_48);
        func_0206e9d4(auStack_48,auStack_48,0,0,-(iVar2 + -0x8e0));
        func_0206f77c(auStack_48);
        func_0205c9bc(0,0,0,0,0,0,0,0);
        func_0205c9f8(0,0,3,0x3f,0x1f,0);
        func_0205ca1c(uVar1);
        func_0205ca28(2);
        iVar2 = func_0207b694((int)*(short *)(param_1 + 0x4a) << 0xc,3);
        uVar1 = ((unsigned int)0xfffff000u);
        iVar2 = (iVar2 << 0xb) >> 0x10;
        func_0205ca34(((unsigned int)0xfffff000u),iVar2,0);
        func_0205ca34(0x1000,iVar2,0);
        func_0205ca34(uVar1,iVar2,0);
        func_0205ca34(0x1000,iVar2,0);
        func_0205ca50();
    }
    return;
}

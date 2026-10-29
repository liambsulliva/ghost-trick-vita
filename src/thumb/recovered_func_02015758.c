extern int func_02009900();
extern int func_0201577c();
extern int func_0202fb48();

void func_02015758(unsigned int param_1)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02009900();
    iVar2 = func_0202fb48(uVar1,((unsigned int)0x020b9cb0u),0xffffffff);
    if (iVar2 != 0) {
        func_0201577c(iVar2,param_1);
    }
    return;
}

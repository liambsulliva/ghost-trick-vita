extern int func_02009900();
extern int func_0200a470();
extern int func_0202fb48();

void func_0200a454(void)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02009900();
    iVar2 = func_0202fb48(uVar1,((unsigned int)0x020b42bcu),0xe);
    if (iVar2 != 0) {
        func_0200a470();
    }
    return;
}

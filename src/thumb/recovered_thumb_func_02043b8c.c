extern int func_02008418();
extern int func_02009900();
extern int func_02010dc4();
extern int func_0202fb48();

void func_02043b8c(unsigned int param_1,int param_2)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02009900();
    iVar2 = func_0202fb48(uVar1,((unsigned int)0x020ce7a0u),0xffffffff);
    if (iVar2 != 0) {
        if (param_2 != 0) {
            func_02010dc4();
            return;
        }
        func_02008418();
    }
    return;
}

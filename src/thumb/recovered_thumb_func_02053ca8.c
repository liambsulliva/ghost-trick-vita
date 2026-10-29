extern int func_0201003c();
extern int func_02010b54();
extern int func_02015178();
extern int func_0203fdd8();
extern int func_0205964c();

void func_02053ca8(int param_1)

{
    int iVar1;

    func_0205964c();
    iVar1 = func_02010b54(param_1);
    if (iVar1 != 0) {
        func_0201003c();
        iVar1 = func_02015178();
        if (iVar1 != 0) {
            func_0203fdd8(param_1 + 0x28,2);
            return;
        }
    }
    func_0203fdd8(param_1 + 0x28,0);
    return;
}

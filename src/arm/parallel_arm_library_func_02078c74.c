extern int func_0206f4cc();
extern int func_0206f4dc();
extern int func_02078a74();

void func_02078c74(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    iVar2 = param_1;
    if (param_1 != 1) {
        iVar2 = 0;
        uVar1 = func_0206f4cc();
        if (uVar1 != 0) {
            func_0206f4dc();
        }
    }
    func_02078a74(iVar2,0,0,1);
    return;
}

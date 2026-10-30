extern void func_0200b518();
extern void func_0200bc44();
extern void func_0203fdd8();
extern void func_02059638();
extern void func_0205963c();

void func_020595fc(int param_1)
{
    unsigned int uVar1;
    int iVar2;

    uVar1 = 0x80000000;
    func_0200b518(param_1 + 0x28, 0, uVar1, uVar1);
    func_0200bc44(param_1 + 0x28);
    func_0203fdd8(param_1 + 0x28, 2);
    iVar2 = param_1 + 0x114;
    func_02059638(iVar2, param_1);
    func_0205963c(param_1 + 0x28, iVar2);
}

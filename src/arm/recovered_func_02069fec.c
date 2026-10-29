typedef int code();
extern int func_02074120();
extern int func_02075c98();
extern int func_02075cac();

void func_02069fec(int param_1)

{
    unsigned int uVar1;
    code *pcVar2;
    unsigned int uVar3;

    pcVar2 = *(code **)(param_1 + 0x4ec);
    uVar3 = *(unsigned int *)(param_1 + 0x4f0);
    uVar1 = func_02075c98();
    *(unsigned int *)(param_1 + 4) = *(unsigned int *)(param_1 + 4) & 0xffffffb3;
    func_02074120(param_1 + 0x4f4);
    func_02075cac(uVar1);
    if (pcVar2 == (code *)0x0) {
        return;
    }
    (*pcVar2)(uVar3);
    return;
}

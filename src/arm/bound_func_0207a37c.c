extern char data_020d34f0;
typedef int code();
extern int func_0207a3c0();
extern int func_0207d9ac();

void func_0207a37c(unsigned int param_1)

{
    int iVar1;
    code *pcVar2;

    iVar1 = ((int)&data_020d34f0);
    if (*(int *)(((int)&data_020d34f0) + 0xc) == 0) {
        func_0207d9ac();
        pcVar2 = *(code **)(iVar1 + 4);
        if (pcVar2 != (code *)0x0) {
            (*pcVar2)();
            *(unsigned int *)(iVar1 + 4) = 0;
        }
    }
    func_0207a3c0(param_1);
    return;
}

extern unsigned int func_02018dd4();
extern unsigned int func_02018e0c();
extern int func_02019358();
extern void func_02018e9c();

void func_02018740(int *param_1)
{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;

    uVar1 = func_02018dd4(param_1);
    uVar2 = func_02018e0c((int)param_1,uVar1);
    if (uVar2 != 1) {
        if (*(char *)((int)param_1 + 0x8f) != 0) {
            iVar3 = func_02019358((unsigned char *)param_1);
            if (iVar3 == 0) {
                *(unsigned char *)((int)param_1 + 0x59) = 1;
            }
        }
        else if ((uVar1 & 0x800000) != 0) {
            func_02018e9c((int)param_1);
        }
    }
    return;
}

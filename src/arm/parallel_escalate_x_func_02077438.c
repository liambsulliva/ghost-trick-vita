extern void func_020768a8();
extern void func_020768bc();
extern void func_02074860();

void func_02077438(int param_1)
{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;
    int iVar4;
    unsigned int uVar5;
    int iVar6;
    unsigned int next;

    func_020768a8();
    for (iVar6 = 0; iVar6 < 4; iVar6++) {
        iVar3 = param_1 + iVar6 * 8;
        uVar2 = *(unsigned int *)(iVar3 + 0x18);
        if (uVar2 != 0) {
            uVar5 = param_1 + 0x18 + iVar6 * 8;
            uVar1 = *(unsigned int *)(uVar2 + 0x18);
            if (uVar5 == uVar1) {
                iVar4 = 0x3c;
                *(unsigned int *)(uVar2 + 0x18) = *(unsigned int *)(iVar3 + 0x1c);
                func_02074860(uVar2, iVar4);
            }
            else {
                if (uVar1 != 0) {
                    do {
                        next = *(unsigned int *)(uVar1 + 4);
                        if (uVar5 == next) {
                            break;
                        }
                        uVar1 = next;
                    } while (next != 0);
                }
                uVar2 = uVar1;
                iVar4 = 8;
                *(unsigned int *)(uVar1 + 4) = *(unsigned int *)((unsigned int)param_1 + iVar6 * 8 + 0x1c);
                func_02074860(uVar2, iVar4);
            }
        }
    }
    func_020768bc();
}

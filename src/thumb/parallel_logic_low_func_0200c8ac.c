extern void func_0200c8f0();
extern int func_02031254();
extern int func_0200cfb8();
extern void func_0200c71c();
extern void func_0200c728();

void func_0200c8ac(int param_1, int param_2)
{
    int *puVar2;
    int iVar3;

    func_0200c8f0(param_1, param_2);
    puVar2 = (int *)func_02031254(0x2048);
    if (puVar2 != 0) {
        puVar2 = (int *)func_0200cfb8(puVar2);
    }
    if (puVar2 != 0) {
        func_0200c71c(puVar2);
        iVar3 = 0x11;
        if (param_2 != 0) {
            iVar3 = 0x1f;
        }
        func_0200c728(puVar2, iVar3);
        *(int **)(param_1 + param_2 * 4 + 0x1520) = puVar2;
    }
    return;
}

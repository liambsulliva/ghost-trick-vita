extern void func_0200c7a4();
extern int func_02031254();
extern int func_0200cde8();
extern void func_0200c71c();
extern void func_0200c728();

void func_0200c760(int param_1, int param_2)
{
    int *puVar2;
    int iVar3;

    func_0200c7a4(param_1, param_2);
    puVar2 = (int *)func_02031254(0x248);
    if (puVar2 != 0) {
        puVar2 = (int *)func_0200cde8(puVar2);
    }
    if (puVar2 != 0) {
        func_0200c71c(puVar2);
        iVar3 = 4;
        if (param_2 != 0) {
            iVar3 = 0x12;
        }
        func_0200c728(puVar2, iVar3);
        *(int **)(param_1 + param_2 * 4 + 0x1518) = puVar2;
    }
    return;
}

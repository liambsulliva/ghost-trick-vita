extern char data_020af0ac;
extern int func_02056978();
extern void func_0200f70c();
extern int func_02031254();
extern int func_0205e8bc();

int *func_0205c058(int *param_1)
{
    int *puVar1;
    int *puVar2;

    func_02056978(param_1);
    *param_1 = (int)&data_020af0ac;
    param_1[0x13] = 0;
    puVar1 = param_1 + 0x15;
    puVar2 = param_1 + 0x1b;
    do {
        func_0200f70c(puVar1);
        puVar1 = puVar1 + 3;
    } while (puVar1 < puVar2);
    do {
        func_0200f70c(puVar2);
        puVar2 = puVar2 + 3;
    } while (puVar2 < param_1 + 0x1e);
    puVar1 = (int *)func_02031254(0x78c);
    if (puVar1 != (int *)0) {
        puVar1 = (int *)func_0205e8bc(puVar1);
    }
    param_1[0x12] = (int)puVar1;
    return param_1;
}

extern int data_020ceda8;
extern int data_0209c108;
extern void *func_02031254();
extern void *func_0201e2cc();
extern int func_0201e2ec();
typedef int code();

void func_02050a44(void)
{
    void *puVar1;

    if (data_020ceda8 == 0) {
        puVar1 = func_02031254(0x20);
        if (puVar1 != 0) {
            puVar1 = func_0201e2cc(puVar1);
        }
        data_020ceda8 = (int)puVar1;
        func_0201e2ec(puVar1, 0x5c, 0x200, 0, 0);
    }
    (**(code **)(*(int *)data_020ceda8 + 8))((int *)data_020ceda8, 0, &data_0209c108);
    return;
}

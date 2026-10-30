extern int func_02010b54();
extern int func_02031254();
extern int func_020299ec();
extern int func_02029a20();
extern void func_02029a04();
extern void func_02031270();

int *func_02059b88(int param_1)
{
    int *obj;

    if (!func_02010b54(param_1)) {
        return (int *)0;
    }
    obj = (int *)func_02031254(0x410);
    if (obj != (int *)0) {
        obj = (int *)func_020299ec(obj);
    }
    if (obj != (int *)0 && func_02029a20(obj, param_1 + 0x114) == 0) {
        if (obj != (int *)0) {
            func_02029a04(obj);
            func_02031270((int)obj);
        }
        return (int *)0;
    }
    return obj;
}

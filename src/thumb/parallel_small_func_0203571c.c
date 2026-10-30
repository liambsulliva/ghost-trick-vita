extern int func_0202fb38(int *object);
extern void func_0203575c(int *object, int owner);
extern char *func_02008444(void);
extern void func_02031b04(char *object, int *field);

void func_0203571c(int *param_1)
{
    if (param_1[9] != 0) {
        if (func_0202fb38((int *)param_1[9]) != 0) {
            ((void (*)(int *, int *))(*(int *)(*(int *)param_1[9] + 0x20)))((int *)param_1[9], param_1);
        } else {
            func_0203575c((int *)param_1[9], 0);
            ((void (*)(int *))(*(int *)(*(int *)param_1[9] + 0xcc)))((int *)param_1[9]);
        }
        param_1[9] = 0;
    }
    func_02031b04(func_02008444(), param_1 + 10);
}

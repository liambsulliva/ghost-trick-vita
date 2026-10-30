extern int func_02068100();
extern int func_0206812c();
extern int func_020684f0();

struct func_02068654_local
{
    int a;
    int b;
};

void func_02068654(int param_1, int param_2)
{
    struct func_02068654_local local;
    int n;

    n = param_2 - 16;
    func_02068100(&local, n);
    func_0206812c(param_1 + 44, n);
    func_020684f0(param_1 + 36, &local);
}

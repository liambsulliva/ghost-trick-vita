struct Pair {
    int first;
    int second;
};

extern struct Pair data_020acaa0;
extern void func_02058fbc(int param_1);
extern void func_02053134(int param_1, struct Pair value);

void func_020530d4(int param_1)
{
    func_02058fbc(param_1);
    func_02053134(param_1, data_020acaa0);
}

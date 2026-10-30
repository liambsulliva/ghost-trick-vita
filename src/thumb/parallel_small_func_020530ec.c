struct Pair {
    int first;
    int second;
};

extern struct Pair data_020acaa8;
extern void func_02059a0c(int param_1);
extern void func_02053134(int param_1, struct Pair value);

void func_020530ec(int param_1)
{
    func_02059a0c(param_1);
    func_02053134(param_1, data_020acaa8);
}

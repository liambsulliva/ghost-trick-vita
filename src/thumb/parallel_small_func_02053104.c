struct Pair {
    int first;
    int second;
};

extern struct Pair data_020acab0;
extern void func_020593a8(int param_1);
extern void func_02053134(int param_1, struct Pair value);

void func_02053104(int param_1)
{
    func_020593a8(param_1);
    func_02053134(param_1, data_020acab0);
}

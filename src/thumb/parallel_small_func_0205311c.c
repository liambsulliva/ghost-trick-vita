struct Pair {
    int first;
    int second;
};

extern struct Pair data_020acab8;
extern void func_020593e8(int param_1);
extern void func_02053134(int param_1, struct Pair value);

void func_0205311c(int param_1)
{
    func_020593e8(param_1);
    func_02053134(param_1, data_020acab8);
}

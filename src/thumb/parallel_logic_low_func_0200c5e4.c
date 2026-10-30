extern void func_0200bc7c();
extern void func_0200bdcc();

void func_0200c5e4(char *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    int state;

    state = *(int *)(param_1 + 0x1528);
    if (state != 1) {
        if (state != 2) {
            return;
        }
        func_0200bc7c(param_1);
        func_0200bc7c(param_1 + 0xa88);
        *(int *)(param_1 + 0x1528) = 1;
    }
    func_0200bdcc(param_1);
    func_0200bdcc(param_1 + 0xa88);
}

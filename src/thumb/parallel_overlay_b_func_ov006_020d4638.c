extern int func_0200bc64();
extern int func_020280a8();
extern int func_02059530();
extern int func_0205953c();
extern int func_020595fc();

void func_ov006_020d4638(int param_1)
{
    func_020595fc();
    if (*(int *)(param_1 + 0x168) == 0) {
        if (*(signed char *)(param_1 + 0x164) == ',') {
            if (*(unsigned char *)(param_1 + 0x162) == 0) {
                func_02059530(param_1, 0x10);
            }
            else {
                func_0205953c(param_1, 1);
                func_0200bc64(param_1 + 0x28, param_1 + 0x170);
            }
        }
        else {
            func_02059530(param_1, 0x1000);
            func_0205953c(param_1, 0);
            func_0200bc64(param_1 + 0x28, param_1 + 0x16c);
        }
    }
    func_020280a8(param_1 + 0x28, *(unsigned char *)(param_1 + 0x163));
}

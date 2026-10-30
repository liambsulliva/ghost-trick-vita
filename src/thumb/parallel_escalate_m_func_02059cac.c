extern unsigned int *func_02008444(void);
extern void func_0200b598(int param_1, int param_2, int param_3);
extern void func_0200bc64(int param_1, void *param_2);
extern unsigned char func_02010b54(int param_1);
extern int func_0201132c(unsigned int *param_1);
extern int func_02029ab4(int param_1);
extern int func_020328e4(unsigned int *param_1);
extern int func_020595cc(int param_1);
extern void func_02059d20(int param_1);

extern char data_020cf820;
extern char data_020cf81c;
extern char data_020cf824;

void func_02059cac(int param_1)
{
    int value;

    if (func_020595cc(param_1) == 0 && func_02010b54(param_1) != 0) {
        if (func_0201132c(func_02008444()) != 0) {
            func_0200bc64(param_1 + 0x28, &data_020cf824);
        }
        else if (func_020328e4(func_02008444()) != 0) {
            func_0200bc64(param_1 + 0x28, &data_020cf81c);
        }
        else {
            func_0200bc64(param_1 + 0x28, &data_020cf820);
        }
        value = func_02029ab4(param_1 + 0x114);
        func_0200b598(param_1 + 0x28, value, 1);
        return;
    }
    func_02059d20(param_1);
}

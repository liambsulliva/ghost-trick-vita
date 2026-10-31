extern int func_02009900(void);
extern int func_0202fa8c(int a, int b);
extern void func_0202fad4(int a, int b);
extern int func_02011264(void);
extern int func_0201123c(int a, int b);
extern void func_02022f4c(int a);
extern void func_02041dcc(void);
extern void func_02011340(char *object);
extern void func_02011404(char *object);
extern int func_02011644(char *object);
extern int func_020112a8(char *object);
extern void func_02022f34(int a, int b, int c);
struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
extern char data_020ced8c;
extern char data_020cf9fc;
extern char data_02083e50;
void func_02011598(char *param_1)
{
    int mask;
    if (!((((*(unsigned int *)(param_1 + 0xc)) << 7) >> 31) == 0 && func_0202fa8c(func_02009900(), (int)&data_020ced8c) == 0)) {
        mask = (int)0xfeffffff;
        *(unsigned int *)(param_1 + 0xc) &= mask;
        func_0202fad4(func_02009900(), (int)&data_020ced8c);
        func_0202fad4(func_02009900(), (int)&data_020cf9fc);
        func_02022f4c(func_0201123c(func_02011264(), 1));
        data_020c9e48.flags = (unsigned int)(mask >> 12) & data_020c9e48.flags;
        func_02041dcc();
        func_02011340(param_1);
        func_02011404(param_1);
        return;
    }
    if ((((*(unsigned int *)(param_1 + 0xc)) << 9) >> 31) != 0) {
        return;
    }
    if (func_02011644(param_1) != 0) {
        return;
    }
    *(unsigned int *)(param_1 + 0xc) &= 0xfeffffff;
    if (func_020112a8(param_1) != 0) {
        return;
    }
    func_02022f34(func_0201123c(func_02011264(), 1), (int)&data_02083e50, 0);
}

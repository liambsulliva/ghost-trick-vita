extern int func_02010b54(int object);
extern int func_02029640(char *object);
extern int *func_020082f8(int *object, unsigned int name);
extern void func_02029704(int object, int *value);
extern int *func_02008444(void);
extern int func_0200f320(int *object);
extern int func_0202970c(void);
extern int func_02029708(int object);
extern void func_02029030(int object, int a, int b, int c, int d, unsigned short e, int f, int g, int h);
extern char data_020cf388[];

void func_0202966c(char *param_1)
{
    int child;
    int *found;
    int amount;

    if (*(int *)(param_1 + 0x48) != 0 && *(int *)(param_1 + 0x58) != 0 && func_02010b54((int)param_1) != 0) {
        child = func_02029640(param_1);
        if (child == 0 || func_02010b54(child) != 0) {
            if (child != 0 && (found = func_020082f8((int *)child, (unsigned int)data_020cf388)) != 0) {
                func_02029704(*(int *)(param_1 + 0x48), found);
            }
            if (func_0200f320(func_02008444()) != 0) {
                amount = *(int *)(param_1 + 0x58) >> 1;
                if (amount != 0) {
                    func_02029030(*(int *)(param_1 + 0x48), func_02029708(func_0202970c()), 0x7e10, amount, 0x1000,
                                  *(unsigned short *)(param_1 + 0x42), 0, 0, 0);
                }
            } else {
                amount = *(int *)(param_1 + 0x58) * 3 >> 2;
                if (amount != 0) {
                    func_02029030(*(int *)(param_1 + 0x48), func_02029708(func_0202970c()), 0x7e10, amount, 0x1000,
                                  *(unsigned short *)(param_1 + 0x42), 0, 0, 0);
                }
            }
        }
    }
}

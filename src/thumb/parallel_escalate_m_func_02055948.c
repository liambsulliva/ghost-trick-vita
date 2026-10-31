extern void *func_02008444(void);
extern int func_0200f330(void *object);
extern int func_020330a4(void *object);

void func_02055948(char *param_1)
{
    int value;

    if (func_0200f330(func_02008444()) == 0) {
        if (func_020330a4(func_02008444()) != 0 && *(unsigned char *)(param_1 + 0xc4) != 0) {
            value = *(int *)(param_1 + 0xc8) + *(int *)(param_1 + 0xc0);
            *(int *)(param_1 + 0xc8) = value;
            if (value < 0) {
                *(int *)(param_1 + 0xc8) = 0xff000;
            } else if (value > 0xff000) {
                *(int *)(param_1 + 0xc8) = 0;
            }
            *(int *)(param_1 + 0xb0) = *(int *)(param_1 + 0xc8);
        }
    }
}

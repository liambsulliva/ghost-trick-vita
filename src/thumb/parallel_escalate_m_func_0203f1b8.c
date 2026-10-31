extern void func_0203f21c(int param_1);
extern int func_0203f3ac(int param_1);

struct Global {
    char pad[0x54];
    unsigned int lo : 11;
    unsigned int b11 : 1;
};
extern struct Global data_020c9e48;

void func_0203f1b8(int param_1, int param_2, int param_3)
{
    if (*(int *)(param_1 + 0x20) == 0) {
        func_0203f21c(param_1);
    }
    if (func_0203f3ac(param_1) != 0) {
        *(char *)(param_1 + 0x50) = 0;
        data_020c9e48.b11 = 1;
    }
    *(int *)(param_1 + 0x24) = param_2;
    *(int *)(param_1 + 0x28) = param_3;
}

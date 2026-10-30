extern void func_020326a4();
extern unsigned int *func_02008444();
extern void func_02011080();
extern void func_0205e22c();

struct flags38 {
    char pad[0x38];
    unsigned int low : 2;
    unsigned int high : 1;
};

void func_0203443c(int param_1, int param_2, int param_3, int param_4)
{
    unsigned int *puVar1;

    func_020326a4(param_1, param_2, param_3, param_4);
    puVar1 = func_02008444();
    func_02011080((int)puVar1, 0x16);
    ((struct flags38 *)param_1)->low = 0;
    ((struct flags38 *)param_1)->high = 0;
    *(unsigned char *)(param_1 + 0x3c) = 0;
    *(unsigned char *)(param_1 + 0x3d) = 0;
    *(unsigned char *)(param_1 + 0x4c) = 0;
    *(unsigned short *)(param_1 + 0x3e) = 0;
    func_0205e22c(*(int *)(param_1 + 0x20), 1);
}

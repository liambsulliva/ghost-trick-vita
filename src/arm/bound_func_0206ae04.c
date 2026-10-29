extern char data_020d02a0;
extern char data_020d08e0;
typedef int code();
extern int func_0206a97c();
extern int func_0206aa3c();
extern int func_0206b168();

void func_0206ae04(int param_1)

{
    (**(code **)(((int)&data_020d08e0) + 0xc))
                        (0,*(unsigned int *)(param_1 + 0x500),*(unsigned int *)(param_1 + 0x4fc),
                          *(unsigned int *)(param_1 + 0x504));
    func_0206a97c();
    func_0206b168();
    func_0206aa3c(8);
    *(unsigned int *)*((unsigned int *)&data_020d02a0) = 0;
    return;
}

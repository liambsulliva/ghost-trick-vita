extern char data_020aa8f0;
extern char data_020cfca0;
typedef int code();
extern int func_02009900();
extern int func_0200f70c();
extern int func_020153d8();
extern int func_0202f7e8();
extern int func_02047120();
extern int func_0204da30();
extern int func_02061d64();

unsigned int * func_0204da38(unsigned int *param_1)

{
    unsigned int uVar1;

    func_02047120();
    *param_1 = ((unsigned int)&data_020aa8f0);
    param_1[0x2e] = 0;
    func_0200f70c(param_1 + 0x2f);
    func_0200f70c(param_1 + 0x32);
    uVar1 = func_02061d64(((unsigned int)&data_020cfca0));
    param_1[0x2e] = uVar1;
    uVar1 = func_02009900();
    func_0202f7e8(uVar1,param_1[0x2e],7,0);
    (**(code **)(*(int *)param_1[0x2e] + 0xb8))((int *)param_1[0x2e],1);
    func_0204da30(param_1[0x2e],1);
    func_020153d8(param_1[0x2e],param_1);
    return param_1;
}

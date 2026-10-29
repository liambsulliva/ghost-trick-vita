extern char data_020a2fb0;
extern int func_02007aa8();
extern int func_0200e5e8();
extern int func_0200f70c();
extern int func_02012674();
extern int func_02012684();

unsigned int * func_02012620(unsigned int *param_1)

{
    int iVar1;

    func_02007aa8();
    iVar1 = ((int)0x00000d8cu);
    *param_1 = ((unsigned int)&data_020a2fb0);
    func_02012674((int)param_1 + iVar1);
    func_0200f70c((int)param_1 + iVar1 + 0x48);
    func_0200e5e8((int)param_1 + iVar1 + 0x54);
    *(unsigned int *)((int)param_1 + iVar1 + 200) = 0;
    *(unsigned int *)((int)param_1 + iVar1 + 0xc4) = 0;
    *(unsigned int *)((int)param_1 + iVar1 + 0xc0) = 0;
    *(unsigned int *)((int)param_1 + iVar1 + 0xbc) = 0;
    func_02012684(param_1);
    return param_1;
}

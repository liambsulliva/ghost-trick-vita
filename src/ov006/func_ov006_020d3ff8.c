extern int func_02008444();
extern int func_02009bfc();
extern int func_02009c1c();
extern int func_0200ec24();
extern int func_0200ee80();
extern int func_0200f1a8();
extern int func_0200f9e8();
extern int func_0202f3a4();
extern int func_ov006_020d414c();

void func_ov006_020d3ff8(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02008444();
    func_0200ec24(uVar1,0);
    uVar1 = func_02008444();
    func_0200ee80(uVar1,*(unsigned short *)(param_1 + 0x30));
    uVar1 = func_02008444();
    func_02009bfc(uVar1,param_1 + 0x34);
    uVar1 = func_02008444();
    func_0200f1a8(uVar1,*(unsigned int *)(param_1 + 0x40));
    uVar1 = func_02008444();
    func_0200f9e8(uVar1,0x400000);
    uVar1 = func_02009c1c();
    func_0202f3a4(uVar1,0x10,0);
    uVar1 = func_02009c1c();
    func_0202f3a4(uVar1,4,0);
    func_ov006_020d414c(param_1);
    return;
}

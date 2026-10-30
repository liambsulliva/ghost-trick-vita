extern unsigned int *func_02008444();
extern int func_0200f320();
extern void func_0205580c();
extern void func_0200f70c();
extern void func_020083b0();
extern int func_0200f5c8();
extern int func_0200f5e8();
extern void func_0200ba7c();

void func_020558f8(int param_1)
{
    int aiStack[3];
    int iVar2;

    iVar2 = func_0200f320((int)func_02008444());
    if (iVar2 == 0) {
        func_0205580c(param_1);
    }
    func_0200f70c(aiStack);
    func_020083b0(aiStack, param_1 + 0xb0);
    iVar2 = func_0200f5c8((int)func_02008444());
    aiStack[0] = aiStack[0] - iVar2;
    iVar2 = func_0200f5e8((int)func_02008444());
    aiStack[1] = aiStack[1] - iVar2;
    func_0200ba7c(param_1 + 0x28, aiStack);
}

extern int func_0200ecb0();
extern int func_0200eeb4();
extern int func_0200eebc();

void func_0200ee80(int param_1,unsigned int param_2)

{
    int iVar1;

    iVar1 = ((int)0x00000de4u);
    if (param_2 == 0xffffffff) {
        param_2 = (unsigned int)*(unsigned short *)(param_1 + 0xd90);
    }
    func_0200eeb4(param_1 + ((int)0x00000de4u));
    func_0200ecb0(param_1,param_2,0x80000);
    func_0200eebc(param_1 + iVar1);
    return;
}

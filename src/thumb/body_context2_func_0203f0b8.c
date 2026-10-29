struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
extern int func_0202f64c();
extern int func_02071960();
extern int func_02071dd0();
extern unsigned short func_02071e40();

void func_0203f0b8(int param_1)

{
    unsigned short uVar1;

    data_020c9e48.flags = ((unsigned int)0xfffff7ffu) & data_020c9e48.flags;
    uVar1 = func_02071e40();
    if (*(unsigned int *)(param_1 + 0x48) != (unsigned int)uVar1) {
        func_02071dd0();
        func_02071960(*(int *)(param_1 + 0x48));
    }
    func_0202f64c((unsigned char *)(param_1 + 0x4c));
    return;
}

struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;

void func_ov004_020d59d4(int param_1, int param_2)
{
    if (param_2 == 4) {
        data_020c9e48.flags = ((unsigned int)0xffffefffu) & (data_020c9e48.flags | 0x400u);
    } else {
        data_020c9e48.flags = ((unsigned int)0xfffffbffu) & (data_020c9e48.flags | 0x1000u);
    }
}

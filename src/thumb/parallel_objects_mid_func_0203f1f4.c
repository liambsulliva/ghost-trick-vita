struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
void func_0203f1f4(void)
{
    data_020c9e48.flags = ((unsigned int)0xfffff7ffu) & data_020c9e48.flags;
}

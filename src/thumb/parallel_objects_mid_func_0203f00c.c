struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
void func_0203f00c(unsigned char *param_1)
{
    data_020c9e48.flags = ((unsigned int)0xfffff0ffu) & data_020c9e48.flags;
    param_1[4] = 0;
}

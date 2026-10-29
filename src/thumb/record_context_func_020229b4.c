struct State {
    unsigned int before_flags[4];
    unsigned int flags;
};
extern struct State data_020c9d50;

void func_020229b4(unsigned int value)
{
    data_020c9d50.flags |= value;
}

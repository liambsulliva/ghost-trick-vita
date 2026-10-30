struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
struct Sound {
    unsigned short before[15];
    unsigned short mode;
};
extern struct State data_020c9e48;
extern struct Sound data_020c9e88;
extern void func_02040904(int param_1);
extern void func_020409ac(int param_1);
extern void func_02040a14(int param_1);

void func_020408c4(int param_1)
{
    data_020c9e48.flags = data_020c9e48.flags & 0x3ffffdff | 0x80001800;
    data_020c9e88.mode = 0x2c3;
    func_020409ac(param_1);
    func_02040a14(param_1);
    func_02040904(param_1);
}

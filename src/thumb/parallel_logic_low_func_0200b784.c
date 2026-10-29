struct Flags {
    unsigned char before : 2;
    unsigned char flag : 1;
};
void func_0200b784(int param_1,unsigned char param_2)

{
    ((struct Flags *)(param_1 + 0x1c))->flag = param_2;
    return;
}

struct Flags {
    unsigned int before : 16;
    unsigned int value : 8;
};
void func_02011080(int param_1,unsigned int param_2)

{
    ((struct Flags *)(param_1 + 8))->value = param_2;
    return;
}

struct Fields {
    unsigned char before_flag[188];
    unsigned char flag;
};
extern int func_0200bbd0();

void func_02055820(int param_1)

{
    if (((struct Fields *)param_1)->flag == '\0') {
        ((struct Fields *)param_1)->flag = 1;
        func_0200bbd0(param_1 + 0x28);
    }
    return;
}

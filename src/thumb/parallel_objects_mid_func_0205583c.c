struct Fields {
    unsigned char before_flag[188];
    unsigned char flag;
};
extern int func_0200b598();
extern int func_0200ba28();

void func_0205583c(int param_1)

{
    unsigned int uVar1;

    if (((struct Fields *)param_1)->flag != '\0') {
        ((struct Fields *)param_1)->flag = 0;
        uVar1 = func_0200ba28((unsigned char *)(param_1 + 0x28));
        func_0200b598(param_1 + 0x28,uVar1,3);
    }
    return;
}

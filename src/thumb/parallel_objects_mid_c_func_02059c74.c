extern char data_0209d9b4;
extern char data_0209d9bc;
extern char data_0209d9c4;
extern char data_0209d9cc;
extern int func_0200da04();

void func_02059c74(int param_1, unsigned int param_2, int param_3)
{
    if (param_3 != 0) {
        func_0200da04(*(char **)(param_1 + 0xe0), ((int)&data_0209d9b4), ((char *)&data_0209d9bc), ((unsigned int)&data_0209d9c4));
        return;
    }
    func_0200da04(*(char **)(param_1 + 0xe0), ((int)&data_0209d9b4), ((char *)&data_0209d9cc), ((unsigned int)&data_0209d9c4));
    return;
}

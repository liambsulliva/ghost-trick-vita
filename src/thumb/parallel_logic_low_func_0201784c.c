extern int func_0200dc3c(int param_1,int param_2);

void func_0201784c(int param_1,unsigned int param_2)
{
    int result;

    result = func_0200dc3c(0x020b9d6c,param_2);
    if (result != 0 && ((int *)param_1)[24] != result) {
        ((int *)param_1)[24] = result;
    }
}

extern int func_020379f8();
extern int func_0203971c();

void func_02039774(int param_1)

{
    if ((*(signed char *)(param_1 + 0xde) == '\x02') &&
          ((unsigned int)*(unsigned char *)(param_1 + 0xdc) < *(unsigned int *)(param_1 + 0xd8))) {
        func_0203971c((unsigned char *)param_1,0);
        func_020379f8(param_1);
    }
    return;
}

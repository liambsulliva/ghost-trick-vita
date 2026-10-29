extern char data_020b18a0;
void func_02003d0c(unsigned int param_1,unsigned char param_2)

{
    if (param_1 < 0x20) {
        *(unsigned char *)(*(int *)(((int)&data_020b18a0) + 0x10) + param_1 * 8 + ((int)0x00001a82u)) = param_2 & 0x7f;
    }
    return;
}

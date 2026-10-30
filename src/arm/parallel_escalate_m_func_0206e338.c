extern char data_020d0ddc;
void func_02069bc0(unsigned int);
void func_02069c30(unsigned int);

int func_0206e338(int param_1, int param_2)
{
    switch (param_2) {
    case 9:
        func_02069bc0((unsigned short)*(unsigned int *)(&data_020d0ddc + 4));
        return 0;
    case 10:
        func_02069c30((unsigned short)*(unsigned int *)(&data_020d0ddc + 4));
        return 0;
    case 1:
        return 4;
    }
    return 0x102;
}

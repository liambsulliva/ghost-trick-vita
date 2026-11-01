extern unsigned char data_02080e8a[];

int func_0202af00(int unused, int index)
{
    return (data_02080e8a[index * 8] & 1) != 0;
}

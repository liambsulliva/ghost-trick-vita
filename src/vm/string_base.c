extern char data_020ca00c;
char *func_020223b4(void *table);

char *func_02027c28(int offset)
{
    return func_020223b4(&data_020ca00c) + offset;
}

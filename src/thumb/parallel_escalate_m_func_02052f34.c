struct Table {
    int values[4];
};
extern struct Table data_0209c438;
extern signed char data_0209d4b8[];
extern signed char data_0209d4c0[];
extern signed char data_0209d4c8[];
extern void func_0200da04();
void func_02052f34(char *object, int index, int flag)
{
    struct Table table;
    table = data_0209c438;
    if (flag != 0) {
        func_0200da04(*(signed char **)(object + 0xe0), table.values[index], data_0209d4b8, data_0209d4c0);
    } else {
        func_0200da04(*(signed char **)(object + 0xe0), table.values[index], data_0209d4c8, data_0209d4c0);
    }
}

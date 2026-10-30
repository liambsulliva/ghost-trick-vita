extern char data_02080e84;
extern char data_02080e86;
extern int func_02007118(int index);
extern int func_02007130(int kind,int value,int index);

int func_0202b014(int param_1,int param_2)
{
    int first;
    int entry;
    int result;

    entry = *(int *)(param_1 + param_2 * 0xc + 0x1ec);
    first = func_02007118(param_2);
    entry = entry * 8;
    result = func_02007130(*(unsigned short *)(((int)&data_02080e86) + entry) & 0xff,
                           *(unsigned short *)(((int)&data_02080e84) + entry),param_2);
    if (result == 0) {
        return 2;
    }
    if (result == 1) {
        return 3;
    }
    if (result == 2) {
        return 4;
    }
    if (result == 3) {
        switch (first) {
        case 0:
            return 1;
        }
        return 0;
    }
    return 0;
}

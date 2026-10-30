extern unsigned int *func_02008444(void);
extern int func_0201ff10(char *, int);
extern int func_0201fcf4(int, int);
int func_ov006_020d7504(void *unused, int value)
{
    int item = func_0201ff10((char *)func_02008444(), 0);
    if (item != 0 && func_0201fcf4(item, value + 1) != 0) return 1;
    item = func_0201ff10((char *)func_02008444(), 2);
    if (item != 0 && func_0201fcf4(item, value + 1) != 0) return 1;
    return 0;
}

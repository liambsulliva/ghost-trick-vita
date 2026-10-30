extern unsigned int *func_02008444(void);
extern int func_0201ff10(char *, int);
extern int func_0201f944(int, int);
int func_ov006_020d74a4(void *unused, int a, int b)
{
    int item = func_0201ff10((char *)func_02008444(), a + 1);
    if (item != 0) return func_0201f944(item, b + 1);
    return -1;
}

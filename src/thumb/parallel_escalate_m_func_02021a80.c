extern char *func_02078e84(char *destp, const signed char *srcp);
extern int func_02079020(signed char *s);
extern int func_020790c8();

char *func_02021a80(int param_1, char *param_2)
{
    char *buf;
    unsigned int size;
    unsigned int i;
    char *buf2;
    unsigned int size2;

    buf = *(char **)(param_1 + 0x14);
    size = *(unsigned int *)(param_1 + 0x18);
    i = 0;
    while (i < size) {
        if (func_020790c8(param_2, buf + i) == 0) {
            return buf + i;
        }
        i += func_02079020((signed char *)(buf + i)) + 1;
    }
    buf2 = *(char **)(param_1 + 0x14);
    size2 = *(unsigned int *)(param_1 + 0x18);
    func_02078e84(buf2 + size2, (const signed char *)param_2);
    *(unsigned int *)(param_1 + 0x18) += func_02079020((signed char *)param_2) + 1;
    return buf2 + size2;
}

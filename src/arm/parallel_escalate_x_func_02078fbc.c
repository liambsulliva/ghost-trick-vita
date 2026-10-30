char *func_02078fbc(const signed char *srcp, const signed char *str)
{
    int j;
    const signed char *p;
    int c;
    int i;
    int d;

    for (i = 0; srcp[i] != 0; i++) {
        p = srcp + i;
        for (j = 0; (c = str[j]) != 0 && (d = p[j], d == c); j++) {
        }
        if (c == 0) {
            return (char *)srcp + i;
        }
    }
    return 0;
}

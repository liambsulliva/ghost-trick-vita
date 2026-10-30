int *func_02073bf0(int *l)
{
    int *p;
    int *next;
    p = (int *)l[0];
    if (p != 0) {
        next = (int *)p[4];
        l[0] = (int)next;
        if (next != 0) {
            next[5] = 0;
        } else {
            l[1] = 0;
        }
    }
    return p;
}

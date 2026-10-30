int *func_02073b68(int *l)
{
    int *p;
    int *next;
    p = (int *)l[0];
    if (p != 0) {
        next = (int *)p[32];
        l[0] = (int)next;
        if (next != 0) {
            next[31] = 0;
        } else {
            l[1] = 0;
            p[30] = 0;
        }
    }
    return p;
}

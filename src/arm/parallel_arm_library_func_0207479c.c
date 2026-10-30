void func_0207479c(int *l, int *n)
{
    int *head;
    head = (int *)l[35];
    if (head == 0) {
        l[34] = (int)n;
    } else {
        head[4] = (int)n;
    }
    n[5] = (int)head;
    n[4] = 0;
    l[35] = (int)n;
}

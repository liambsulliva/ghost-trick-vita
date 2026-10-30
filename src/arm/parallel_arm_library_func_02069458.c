struct Table
{
    unsigned short count;
    unsigned short pad;
    int *base;
};

int func_02069458(struct Table *t, unsigned int n)
{
    if (t->count > n) {
        return (int)t->base + (n << 4);
    }
    return 0;
}

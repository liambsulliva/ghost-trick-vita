extern int func_020225e0(void *p, unsigned int b, int c);

int func_02022c48(void *a, unsigned int b)
{
    if (b == 0) {
        return -1;
    }
    return func_020225e0((char *)a + 0xf70, b, 0);
}

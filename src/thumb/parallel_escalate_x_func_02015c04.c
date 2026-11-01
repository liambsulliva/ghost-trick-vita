typedef int (*Method15c04)(void *self, int a, int b, int c);

int func_02015c04(void **object, int a, int b, int c)
{
    void *inner = object[1];
    return (*(Method15c04 *)(*(char **)inner + 0x98))(inner, a, b, c);
}

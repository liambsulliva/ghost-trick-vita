void func_02074564(int *s)
{
    int v;
    v = s[3];
    s[1] = 0;
    s[0] = 0;
    s[2] = 0;
    v = v & 0xff000000;
    s[3] = v & 0x00ffffff;
}

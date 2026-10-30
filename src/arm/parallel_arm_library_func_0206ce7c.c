extern void func_0206d714();

void func_0206ce7c(unsigned int *obj, unsigned short v)
{
    unsigned int prev;

    prev = obj[2];
    *(unsigned short *)((unsigned char *)obj + 0x34) = v;
    obj[12] = prev;
    *(unsigned short *)((unsigned char *)obj + 0x36) = 0;
    obj[14] = 0;
    func_0206d714(obj, 2, 1);
}

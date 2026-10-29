extern void func_0200b598(unsigned int *, unsigned int, unsigned int);
extern void func_0200b7c0(unsigned int *);

void func_02067a54(unsigned int *object)
{
    unsigned char *p;
    unsigned char b;

    p = (unsigned char *)object;
    b = p[0xbc];
    if (b != p[0xbd]) {
        p[0xbd] = b;
        func_0200b598((unsigned int *)(p + 0x28), b + 0x69, 0xf);
    }
    func_0200b7c0((unsigned int *)(p + 0x28));
}

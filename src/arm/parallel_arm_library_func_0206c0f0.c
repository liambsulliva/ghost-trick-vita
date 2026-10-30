extern void func_0206c440();
extern void func_0206b3a8();

unsigned int func_0206c0f0(unsigned int *obj, unsigned int a, unsigned int b)
{
    unsigned int buf[18];

    obj[8] = a;
    obj[9] = b;
    func_0206c440(buf);
    buf[2] = (unsigned int)obj;
    func_0206b3a8(buf, 17);
    obj[5] = obj[5] | 2;
    return 1;
}

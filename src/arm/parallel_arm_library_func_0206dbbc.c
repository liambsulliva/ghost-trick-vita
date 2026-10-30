extern void func_0206c440();
extern void func_0206d714();

void func_0206dbbc(unsigned int v)
{
    unsigned int buf[18];

    func_0206c440(buf);
    buf[2] = v;
    func_0206d714(buf, 10, 0);
}

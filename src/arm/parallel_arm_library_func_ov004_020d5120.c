void func_ov004_020d5120(unsigned int *obj)
{
    void (*fn)(unsigned int *);

    fn = (void (*)(unsigned int *))((unsigned int *)*obj)[5];
    fn(obj);
}

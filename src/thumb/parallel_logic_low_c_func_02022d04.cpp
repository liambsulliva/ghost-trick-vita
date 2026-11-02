struct Dispatch02022d04 {
    virtual void v000();
    virtual void v004();
    virtual int v008(int value, int flag);
};

extern "C" int func_02022d04(void *owner, int a, int b)
{
    return ((Dispatch02022d04 *)((char *)owner + 0xfc0))->v008(a, b);
}

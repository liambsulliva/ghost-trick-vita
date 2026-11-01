struct Conv0d114 {
    virtual void v0();
    virtual void v4();
    virtual unsigned short v8(unsigned short value, int arg);
};

extern "C" void func_0200d114(Conv0d114 *conv, const unsigned short *src, unsigned short *dst, unsigned int size, int arg)
{
    size = size >> 1;
    while (size != 0) {
        *dst = conv->v8(*src++, arg);
        dst++;
        size--;
    }
}

struct Obj0200d06c {
    virtual void v0();
    virtual void v4();
    virtual int v8(int arg);
    char pad[0x40];
    int stride;
    unsigned char buffer[4];
};

extern "C" unsigned char *func_0200d06c(Obj0200d06c *object, int arg)
{
    int n = object->v8(arg);
    return object->buffer + ((n * object->stride) & ~1);
}

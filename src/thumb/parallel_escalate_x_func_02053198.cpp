struct Obj53198 {
    virtual void v0();
    char pad[0x151];
    unsigned char busy;
};

typedef void (Obj53198::*Handler53198)();

extern "C" void func_02053198(Obj53198 *object, Handler53198 handler)
{
    object->busy = 1;
    (object->*handler)();
    object->busy = 0;
}

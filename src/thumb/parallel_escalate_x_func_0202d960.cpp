struct Obj0202d960 {
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02c();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03c();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04c();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05c();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06c();
    virtual void v070(int arg);
    char pad[0x82];
    unsigned char ready : 1;
    unsigned char shown : 1;
};

extern "C" void func_0202d960(Obj0202d960 *object, int arg)
{
    object->v070(arg);
    object->shown = 1;
    object->ready = 1;
}

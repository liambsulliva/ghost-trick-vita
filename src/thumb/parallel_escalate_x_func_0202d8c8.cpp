struct Obj2d8c8 {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c(int arg);
    char pad[0x82];
    unsigned char ready : 1;
};

extern "C" void func_0202d8c8(Obj2d8c8 *object, int arg)
{
    object->v5c(arg);
    object->ready = 1;
}

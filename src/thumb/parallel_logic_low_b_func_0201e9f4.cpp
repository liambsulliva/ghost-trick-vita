struct Stream {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6(void *buffer,int size);
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual unsigned int slot11();
};

struct Holder {
    int before;
    Stream *stream;
};

extern "C" int func_0201e9f4(Holder *object)
{
    unsigned char buffer[4];

    object->stream->slot11();
    object->stream->slot6(buffer,1);
    return (signed char)buffer[0];
}

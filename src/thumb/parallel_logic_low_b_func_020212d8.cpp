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

extern "C" unsigned long long func_020212d8(Holder *object)
{
    unsigned char pad[8];
    unsigned long long value;
    int align;

    align = object->stream->slot11() & 7;
    if (align != 0) {
        align = 8 - align;
    }
    if (align != 0) {
        object->stream->slot6(pad,align);
    }
    object->stream->slot6(&value,8);
    return value;
}

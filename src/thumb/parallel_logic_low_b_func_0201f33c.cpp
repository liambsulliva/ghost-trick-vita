struct Stream {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13(int size);
    unsigned int position;
    unsigned int length;
    void *data;
    unsigned char owned;
    unsigned char pad[3];
    void *allocator;
};
extern "C" void *func_0201f32c(void *object);
extern "C" void *func_02007bd8(void);
extern "C" void *func_02007b70(void *object);
extern "C" int data_020a4758;

extern "C" Stream *func_0201f33c(Stream *object,void *allocator,unsigned int size,void *data)
{
    func_0201f32c(object);
    *(void **)object = &data_020a4758;
    object->position = 0;
    object->data = 0;
    if (allocator == 0) {
        object->allocator = func_02007b70(func_02007bd8());
    } else {
        object->allocator = allocator;
    }
    if (data != 0) {
        object->owned = 0;
        object->data = data;
        object->length = size;
    } else {
        object->owned = 1;
        object->slot13(size);
    }
    return object;
}

struct Allocator {
    virtual void slot0();
    virtual void slot1();
    virtual void *slot2(unsigned int size,void *tag);
    virtual void slot3();
    virtual void slot4(void *data);
};
struct Stream {
    void *table;
    unsigned int position;
    unsigned int length;
    void *data;
    unsigned char owned;
    unsigned char pad[3];
    Allocator *allocator;
};
extern "C" char data_02084e40;

extern "C" void func_0201f480(Stream *object,unsigned int size)
{
    if (object->owned != 0 && object->data != 0 && object->allocator != 0) {
        object->allocator->slot4(object->data);
        object->data = 0;
    }
    object->owned = 1;
    object->data = object->allocator->slot2(size,&data_02084e40);
    if (object->data != 0) {
        object->length = size;
    } else {
        object->length = 0;
    }
}

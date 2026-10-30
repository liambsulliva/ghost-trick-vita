struct Storage {
    unsigned int first;
    int count;
    unsigned int stride;
    unsigned int data;
    unsigned int reserved[2];
    unsigned char external;
    unsigned char padding[3];
    unsigned int cursor;
};
extern void func_0201e35c(struct Storage *storage);
extern void func_02031270(unsigned int data);
extern unsigned int func_02031288(unsigned int size);

int func_0201e2ec(struct Storage *storage, int size, int count, unsigned int buffer, unsigned int capacity)
{
    if (storage->data != 0 && storage->external == 0) {
        func_02031270(storage->data);
        storage->data = 0;
    }
    storage->count = count;
    storage->stride = size + 3U & 0xfffffffc;
    storage->cursor = 0;
    if (buffer != 0 && (unsigned int)(size * count) <= capacity) {
        storage->data = buffer;
        storage->external = 1;
    }
    if (storage->data == 0) {
        storage->data = func_02031288(storage->stride * storage->count & 0xfffffffc);
        storage->external = 0;
    }
    if (storage->data == 0) {
        return 0;
    }
    func_0201e35c(storage);
    return 1;
}

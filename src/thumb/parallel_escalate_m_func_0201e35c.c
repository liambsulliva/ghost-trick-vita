struct Storage {
    unsigned int first;
    int count;
    unsigned int stride;
    unsigned char *data;
    unsigned char *head;
    unsigned char *tail;
    unsigned char external;
    unsigned char padding[3];
    unsigned int cursor;
};
extern void func_0201e3c0(void *data, unsigned int size);

void func_0201e35c(struct Storage *storage)
{
    unsigned char *block;
    unsigned char *next;
    unsigned char *data;
    unsigned int i;

    block = storage->data;
    if (block == 0) {
        return;
    }
    func_0201e3c0(block, storage->stride * storage->count);
    i = 0;
    while (i < storage->count - 1U) {
        next = block + (storage->stride & ~3);
        i++;
        *(unsigned char **)(next - 4) = next;
        block += storage->stride & ~3;
    }
    *(unsigned int *)(block + (storage->stride & ~3) - 4) = 0;
    data = storage->data;
    storage->head = data;
    storage->tail = data;
    if ((unsigned int)storage->count >= 1) {
        storage->tail = data + ((storage->count - 1) * storage->stride & ~3);
    }
    storage->cursor = 0;
}

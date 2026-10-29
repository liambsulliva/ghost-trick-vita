struct Slots {
    unsigned int values[12];
    unsigned int count;
};

void func_0201f29c(struct Slots *object,unsigned int value)
{
    unsigned int index;
    unsigned int count;
    unsigned int current;

    index = 0;
    count = object->count;
    while (index < count) {
        current = object->values[index];
        if (current == value) {
            unsigned int next;
            unsigned int *slot;
            next = index + 1;
            while (next < 11) {
                slot = &object->values[next];
                *slot = slot[1];
                object->count = object->count - 1;
                return;
            }
        }
        index = index + 1;
    }
}

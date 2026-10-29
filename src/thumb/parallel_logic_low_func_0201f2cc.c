typedef int code();

struct Slots {
    int *values[12];
    unsigned int count;
};

void func_0201f2cc(struct Slots *object)
{
    unsigned int index;
    int *entry;

    index = 0;
    while (index < object->count) {
        entry = object->values[index];
        (**(code **)(*entry + 0x1c))(entry);
        index = index + 1;
    }
}

extern void func_0201bee8(unsigned int *object, unsigned int capacity);
void func_0201bec0(unsigned int *object, unsigned int value)
{
    unsigned int count;
    unsigned int capacity;
    capacity = object[1];
    count = object[0];
    if (count < capacity) {
        object[0] = count + 1;
        *(unsigned int *)(object[2] + count * 4) = value;
    } else {
        func_0201bee8(object, (capacity + 1) * 2);
        count = object[0];
        object[0] = count + 1;
        *(unsigned int *)(object[2] + count * 4) = value;
    }
}

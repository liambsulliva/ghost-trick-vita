struct Slots {
    unsigned int values[12];
    unsigned int count;
};

void func_0201f288(struct Slots *object,unsigned int value)
{
    if (object->count < 12) {
        object->values[object->count] = value;
        object->count = object->count + 1;
    }
}

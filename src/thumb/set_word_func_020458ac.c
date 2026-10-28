struct Field {
    unsigned char before[44];
    int value;
};

void func_020458ac(struct Field *object, int value)
{
    object->value = value;
}

struct Field {
    unsigned char before[34];
    unsigned char value;
};

void func_02034258(struct Field *object, int value)
{
    object->value = value;
}

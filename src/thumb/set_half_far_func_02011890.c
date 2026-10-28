struct Field {
    unsigned char before[206];
    unsigned short value;
};

void func_02011890(struct Field *object, int value)
{
    object->value = value;
}

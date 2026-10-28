struct Field {
    unsigned char before[188];
    int value;
};

void func_02040b68(struct Field *object, int value)
{
    object->value = value;
}

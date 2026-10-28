struct Field {
    unsigned char before[68];
    unsigned char value;
};

void func_0200f910(struct Field *object, int value)
{
    object->value = value;
}

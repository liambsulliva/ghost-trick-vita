struct Field {
    unsigned char before[16];
    unsigned char value;
};

void func_020410d0(struct Field *object)
{
    object->value = 2;
}

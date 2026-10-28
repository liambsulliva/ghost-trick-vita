struct Field {
    unsigned char before[4];
    unsigned char value;
};

void func_02009e94(struct Field *object, int value)
{
    object->value = value;
}

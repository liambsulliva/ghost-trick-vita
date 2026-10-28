struct FlagByteE {
    unsigned char before[0xe];
    unsigned char flags;
};

void func_02008418(struct FlagByteE *object)
{
    object->flags = object->flags | 2;
}

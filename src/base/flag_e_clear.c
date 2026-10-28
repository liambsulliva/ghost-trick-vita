struct FlagByteE {
    unsigned char before[0xe];
    unsigned char flags;
};

void func_0200840c(struct FlagByteE *object)
{
    object->flags = object->flags & 0xfe;
}

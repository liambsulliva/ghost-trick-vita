struct FlagByte {
    unsigned char before[4];
    unsigned char low : 1;
    unsigned char rest : 7;
};

void func_0202cb58(struct FlagByte *object)
{
    object->low = 0;
}

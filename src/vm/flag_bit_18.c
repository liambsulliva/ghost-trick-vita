struct FlagWord10 {
    unsigned char before[0x10];
    unsigned int low : 18;
    unsigned int flag : 1;
    unsigned int high : 13;
};

int func_02027ea4(struct FlagWord10 *object)
{
    if (object->flag) {
        return 1;
    }
    return 0;
}

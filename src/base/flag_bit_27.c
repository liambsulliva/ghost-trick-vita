struct FlagWordC {
    unsigned char before[0xc];
    unsigned int low : 27;
    unsigned int flag : 1;
    unsigned int high : 4;
};

int func_02008424(struct FlagWordC *object)
{
    return object->flag;
}

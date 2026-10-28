struct FlagWord8 {
    unsigned char before[0x8];
    unsigned int flag : 1;
    unsigned int rest : 31;
};

int func_02027eb8(struct FlagWord8 *object)
{
    if (object->flag) {
        return 1;
    }
    return 0;
}

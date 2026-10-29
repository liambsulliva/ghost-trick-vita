struct FlagWord {
    unsigned int before : 31;
    unsigned int set : 1;
};

unsigned int func_02023eac(int object)
{
    if (((struct FlagWord *)(object + 8))->set != 0) {
        return 1;
    }
    return 0;
}

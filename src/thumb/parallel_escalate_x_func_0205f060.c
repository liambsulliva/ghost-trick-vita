struct FlagBit27 {
    int pad[4];
    unsigned int low : 27;
    unsigned int flag : 1;
};

int func_0205f060(struct FlagBit27 *param_1)
{
    if (param_1->flag == 1) {
        return 1;
    }
    return 0;
}

struct FlagBit23 {
    int pad[4];
    unsigned int low : 23;
    unsigned int flag : 1;
};

int func_0205f040(struct FlagBit23 *param_1)
{
    if (param_1->flag == 1) {
        return 1;
    }
    return 0;
}

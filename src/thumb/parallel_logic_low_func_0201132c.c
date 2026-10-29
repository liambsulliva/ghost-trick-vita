struct PairWord {
    unsigned int before : 13;
    unsigned int pair : 2;
    unsigned int rest : 17;
};

int func_0201132c(int object)
{
    if (((struct PairWord *)(object + 0xc))->pair == 1) {
        return 1;
    }
    return 0;
}

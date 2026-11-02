struct Block
{
    unsigned int start;
    unsigned int size;
    struct Block *prev;
    struct Block *next;
};

extern int func_020689a8();

int func_02068d08(struct Block **param_1, struct Block **param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned int pair[2];
    struct Block *block;

    pair[0] = param_3;
    pair[1] = param_3 + param_4;
    func_020689a8(param_1, param_2, pair);
    block = *param_2;
    if (block != 0)
    {
        *param_2 = block->next;
    }
    if (block == 0)
    {
        return 0;
    }
    block->start = pair[0];
    block->size = pair[1] - pair[0];
    block->prev = 0;
    block->next = 0;
    if (*param_1 != 0)
    {
        (*param_1)->prev = block;
    }
    block->next = *param_1;
    block->prev = 0;
    *param_1 = block;
    return 1;
}

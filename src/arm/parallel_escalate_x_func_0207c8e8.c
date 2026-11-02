typedef struct Iter0207c8e8 {
    unsigned int start;
    const unsigned char *table;
    const unsigned char *action;
} Iter0207c8e8;

unsigned char func_0207c8e8(Iter0207c8e8 *iter)
{
    return iter->action != 0 ? *iter->action & 0x1f : 0;
}

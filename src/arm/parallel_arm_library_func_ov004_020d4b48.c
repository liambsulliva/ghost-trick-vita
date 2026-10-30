struct State { unsigned int pad[39]; unsigned int index; unsigned int count; unsigned int unused; int limit; unsigned int pad2[6]; int cursor; };

int func_ov004_020d4b48(volatile struct State *p)
{
    int value;
    unsigned int index;

    index = p->index;
    if (index >= p->count) {
        return 0;
    }
    p->index = index + 1;
    value = p->cursor + 1;
    p->cursor = value;
    if (value == p->limit) {
        p->cursor = 0;
    }
    return 1;
}

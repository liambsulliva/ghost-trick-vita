struct Entry { unsigned int value; unsigned int other; };
struct State { int pad[12]; unsigned int count; int pad2[24]; struct Entry *entries; };

unsigned int func_ov004_020d4670(struct State *p, int index)
{
    if (index < 0 || (unsigned int)index >= p->count) {
        return 0;
    }
    return p->entries[index].value;
}

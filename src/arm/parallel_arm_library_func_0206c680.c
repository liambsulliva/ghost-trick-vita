extern unsigned int func_0206e074();
extern unsigned int func_0206b9dc();

unsigned int func_0206c680(unsigned int *obj)
{
    unsigned int out;
    unsigned int slot;
    unsigned int r;

    out = 0;
    r = func_0206e074(obj, &out);
    if (r == 0) {
        obj[4] = (unsigned int)&slot;
        slot = 0;
        r = func_0206b9dc(obj, 16, 1);
        if (r != 0) {
            out = slot;
        }
    }
    return out;
}

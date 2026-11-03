struct Code {
    unsigned char lo;
    unsigned char hi;
};

void func_0201cabc(struct Code *last, const struct Code *code)
{
    last->lo = code->lo;
    last->hi = code->hi;
}

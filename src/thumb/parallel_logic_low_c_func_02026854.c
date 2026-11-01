struct Bits02026854 {
    unsigned int low : 16;
    unsigned int a : 3;
    unsigned int b : 3;
    unsigned int high : 10;
};

struct Holder02026854 {
    unsigned char pad[12];
    struct Bits02026854 word;
};

void func_02026854(struct Holder02026854 *p, unsigned int v)
{
    p->word.b = p->word.a;
    p->word.a = v;
}

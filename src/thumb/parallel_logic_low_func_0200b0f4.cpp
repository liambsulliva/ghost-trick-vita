extern "C" {
class Handler {
public:
    virtual void s0();
    virtual void s1();
    virtual unsigned int s2(unsigned int arg);
};

struct Cell {
    unsigned int w0;
    unsigned int arg;
    unsigned int w8;
    unsigned char lo : 1;
    unsigned char flag : 1;
    unsigned char rest : 6;
    unsigned char pad[3];
    Handler *obj;
    unsigned int w14;
    unsigned int word18;
};

unsigned short func_0200b0f4(struct Cell *cell)
{
    unsigned int r;
    if (cell->flag) {
        r = cell->word18;
    } else {
        r = cell->obj->s2(cell->arg);
    }
    return (unsigned short)r;
}
}

struct Irq
{
    unsigned short ime;
    unsigned short pad;
    unsigned int pad2;
    unsigned int ie;
    unsigned int iflag;
};

unsigned int func_02073594(unsigned int v)
{
    volatile struct Irq *r;
    unsigned short saved;
    unsigned short dead;
    unsigned int old;

    r = (volatile struct Irq *)0x04000208;
    saved = r->ime;
    r->ime = 0;
    old = r->ie;
    r->ie = v;
    dead = r->ime;
    r->ime = saved;
    return old;
}

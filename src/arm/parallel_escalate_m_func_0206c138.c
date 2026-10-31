struct Archive {
    unsigned int header[3];
    unsigned int queue[2];
    unsigned int flag;
};

extern unsigned int func_02075c98(void);
extern unsigned int func_02075cac(unsigned int bits);
extern void func_020740d0(void *queue);

static inline int is_suspended(volatile const struct Archive *arc)
{
    return (arc->flag & 8) ? 1 : 0;
}

int func_0206c138(struct Archive *arc)
{
    int retval;
    unsigned int bak;

    bak = func_02075c98();
    retval = !is_suspended(arc);
    if (retval) {
        if ((arc->flag & 0x10) == 0) {
            arc->flag |= 8;
        } else {
            unsigned int bak2;
            arc->flag |= 0x40;
            bak2 = func_02075c98();
            while (arc->flag & 0x40) {
                func_020740d0(arc->queue);
            }
            func_02075cac(bak2);
        }
    }
    func_02075cac(bak);
    return retval;
}

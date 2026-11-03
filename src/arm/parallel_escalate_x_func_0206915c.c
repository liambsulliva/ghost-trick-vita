extern int data_020cffa8;
extern int data_020cffac;
extern int func_02068b4c(int *mgr, int *list, unsigned int *addr, unsigned int size, unsigned int align);
extern int func_02068d08(int *mgr, int *list, unsigned int addr, unsigned int size);

unsigned int func_0206915c(unsigned int size, int pltt4, unsigned int opt)
{
    unsigned int addr;
    unsigned int sz;
    int ok;

    if (size == 0) {
        sz = 8;
    } else {
        sz = (size + 7) & ~7;
    }
    if (sz >= 0x7fff8) {
        return 0;
    }
    if (pltt4) {
        ok = func_02068b4c(&data_020cffa8, &data_020cffac, &addr, sz, 8);
        if (addr + sz > 0x10000) {
            func_02068d08(&data_020cffa8, &data_020cffac, addr, sz);
            return 0;
        }
    } else {
        ok = func_02068b4c(&data_020cffa8, &data_020cffac, &addr, sz, 16);
    }
    if (ok) {
        return ((sz >> 3) << 16) | ((addr >> 3) & 0xffff);
    } else {
        return 0;
    }
}

struct State {
    unsigned int first;
    unsigned int flags;
    unsigned char mid[1300];
    unsigned int slot;
};
extern struct State data_020d02a0;
extern void func_02074188(unsigned int);

void func_0206a0ec(int kind, unsigned int unused, int extra)
{
    if (kind != 11) {
        return;
    }
    if (extra == 0) {
        return;
    }
    data_020d02a0.flags = data_020d02a0.flags & 0xffffffdf;
    func_02074188(data_020d02a0.slot);
}

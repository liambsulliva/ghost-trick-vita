extern "C" {
struct State {
    unsigned char padding[0xaa8];
    unsigned char flags;
    bool flag : 1;
};
void func_ov004_020d5978(State *);
void func_ov004_020d5958(State *p)
{
    p->flags |= 0x40;
    if (p->flag) {
        func_ov004_020d5978(p);
    }
}
}

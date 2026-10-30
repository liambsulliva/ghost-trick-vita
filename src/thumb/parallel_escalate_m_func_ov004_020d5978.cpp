extern "C" {
void func_02074120(int *);
struct State {
    unsigned char padding[0xaa9];
    bool flag : 1;
};
void func_ov004_020d5978(State *p)
{
    if (!p->flag) {
        p->flag = true;
        return;
    }
    p->flag = false;
    func_02074120((int *)((char *)p + 0x9bc));
}
}

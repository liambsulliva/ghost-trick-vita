struct Pair {
    unsigned short first;
    unsigned short second;
};
struct State {
    unsigned char before[8];
    struct Pair p0;
    struct Pair p1;
    unsigned char mid[40];
    unsigned short flags;
};
extern struct State data_020d2e84;
extern void func_02077eb0(struct Pair *, struct Pair *);

unsigned int func_02077c5c(struct Pair *out)
{
    if ((data_020d2e84.flags & 1) != 0) {
        return 1;
    }
    out[0] = data_020d2e84.p0;
    out[1] = data_020d2e84.p1;
    func_02077eb0(out, out);
    return 0;
}

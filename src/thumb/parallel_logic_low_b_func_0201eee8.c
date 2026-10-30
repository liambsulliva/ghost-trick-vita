extern void func_02020e18(unsigned int *state, unsigned int seed);
extern void func_02020e34(unsigned int *state, unsigned int value);
extern unsigned int func_01ff80b8(unsigned int *state);

unsigned int func_0201eee8(void *unused, signed char *text)
{
    unsigned int acc;
    unsigned int state[4];
    unsigned int i;
    unsigned int n;
    unsigned int call;

    acc = 0;
    i = 0;
    func_02020e18(state, 0x7d4);
    while (text[i] != 0) {
        func_02020e34(state, ((text[i] - 0x20) << 6) | (text[i + 1] - 0x20));
        call = func_01ff80b8(state);
        n = text[i] & 0xf;
        n += call;
        n = (n + 1) & 0xf;
        do {
            func_01ff80b8(state);
        } while (n-- != 0);
        acc ^= func_01ff80b8(state);
        i++;
    }
    return acc % 0x7fffffff;
}

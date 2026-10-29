struct State {
    unsigned int pad;
    unsigned int word4;
    unsigned int word8;
};
extern struct State data_020d0e48;
extern int func_02071bc8(int, int, int);

void func_02070c58(int a, int b, int c)
{
    int v;

    v = func_02071bc8(a, b, c);
    data_020d0e48.word8 = v;
    if (v == 0) {
        return;
    }
    if (v != 0x20) {
        if (v == 0x40) {
            data_020d0e48.word4 = 0x06894000;
        }
        return;
    }
    data_020d0e48.word4 = 0x06890000;
}

struct State {
    unsigned char pad[0x12];
    unsigned char other : 6;
    unsigned char value : 2;
};

void func_02059c0c(struct State *state, unsigned char value)
{
    state->value = value;
}

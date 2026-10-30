typedef struct State {
    char pad[8];
    unsigned int low : 16;
    unsigned int cur : 8;
    unsigned int prev : 8;
} State;

void func_0201192c(State *state, unsigned int value)
{
    if (value != state->cur) {
        state->prev = state->cur;
        state->cur = value;
    }
}

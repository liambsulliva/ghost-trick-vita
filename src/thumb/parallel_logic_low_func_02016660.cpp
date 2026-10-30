extern "C" void *func_020169a4(void *, unsigned int);
struct Object {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void call(int, int, int);
 };
struct State { int unused; Object *current; };
extern "C" void func_02016660(State *state, int a0, int a1, int a2)
{
    state->current = (Object *)func_020169a4(state, 0);
    if (state->current != 0) state->current->call(a0, a1, a2);
 }

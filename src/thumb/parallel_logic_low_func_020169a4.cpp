extern "C" {
void *func_02031254(unsigned int);
void *func_02017878(void *);
unsigned int func_02016a64(void *);
void *func_02019d4c(void *);
void *func_020196f0(void *);
void *func_02019384(void *, int);
void *func_02016d30(void *);
}
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
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual int slot21();
 };
struct State { int unused[3]; Object *primary; Object *current; };
extern "C" Object *func_020169a4(State *state, unsigned int type)
{
    if (type == 0) {
        if (state->primary == 0) {
            void *p = func_02031254(0x206c);
            if (p != 0) p = func_02017878(p);
            state->primary = (Object *)p;
        }
        state->primary->slot7();
        return state->primary;
    }
    if (state->current == 0 || type != func_02016a64(state->current)) {
        if (state->current != 0) {
            if (state->current != 0) state->current->slot1();
            state->current = 0;
        }
        void *p;
        switch (type) {
        case 5: {
            int value = 0;
            if (state->primary != 0) value = state->primary->slot21();
            if (value == 0) break;
            p = func_02031254(0x78);
            if (p != 0) p = func_02019384(p, value);
            state->current = (Object *)p;
            break;
        }
        case 3:
            p = func_02031254(0x78);
            if (p != 0) p = func_020196f0(p);
            state->current = (Object *)p;
            break;
        case 1:
            p = func_02031254(0xf8);
            if (p != 0) p = func_02019d4c(p);
            state->current = (Object *)p;
            break;
        case 6:
        case 7:
            p = func_02031254(0x68);
            if (p != 0) p = func_02016d30(p);
            state->current = (Object *)p;
            break;
        }
    }
    return state->current;
}

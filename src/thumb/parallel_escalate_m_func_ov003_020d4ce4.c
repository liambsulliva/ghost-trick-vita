struct State {
    unsigned char padding[0x154];
    void *items[3];
    unsigned char more[8];
    unsigned char flag;
    unsigned char more2[11];
    unsigned int counter;
    int current;
    int next;
    int count;
};
extern void func_0203bfb4(void *, int);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *, int);
extern void func_ov003_020d4184(struct State *, int);
void func_ov003_020d4ce4(struct State *p, int value)
{
    switch (value) {
    case 1:
    case 2: {
        int next;

        next = p->current - 1 + (value - 1) * 2;
        p->next = next;
        if (next < 0) {
            p->next = p->current;
            return;
        }
        if (next >= p->count) {
            p->next = p->current;
            return;
        }
        func_0203bfb4(p->items[value], 2);
        p->counter = 0;
        p->flag = 2;
        func_0202a0c0(func_0200847c(), 1);
        func_ov003_020d4184(p, p->next);
        break;
    }
    default:
        break;
}
}

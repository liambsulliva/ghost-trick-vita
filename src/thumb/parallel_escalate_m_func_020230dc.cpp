struct Mgr020230dc;

class Node020230dc {
public:
    virtual Node020230dc *slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void *slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void *slot9();
    int words[4];
    unsigned int index;
    int word18;
    Mgr020230dc *owner;
};

extern "C" {
struct Mgr020230dc {
    int word0;
    Node020230dc *items[8];
    unsigned int count;
};

extern char data_020c9db0;
extern int func_0201ee8c(void *object);
extern int func_02008314(void *type, void *kind);
extern void func_02023204(Mgr020230dc *mgr, Node020230dc **slot);

void func_020230dc(Mgr020230dc *mgr, Node020230dc *target)
{
    Node020230dc *stack[8];
    Node020230dc *path[8];
    Node020230dc *cur;
    unsigned int n;
    unsigned int i;
    int j;
    int end;
    int v;

    n = 0;
    cur = target->slot0();
    end = func_0201ee8c(&data_020c9db0);
    v = func_0201ee8c(cur->slot9());
    while (v != end) {
        stack[n] = (Node020230dc *)cur->slot9();
        if (cur != 0) {
            cur->slot1();
        }
        cur = stack[n]->slot0();
        v = func_0201ee8c(cur->slot9());
        n++;
    }
    if (cur != 0) {
        cur->slot1();
    }
    for (i = 0; i < n; i++) {
        path[i] = stack[n - i - 1];
    }
    i = 0;
    if (n != 0) {
        while (i < mgr->count && func_02008314(path[i], mgr->items[i]->slot4()) != 0) {
            i++;
            if (i == n) {
                break;
            }
            if (i == mgr->count) {
                break;
            }
        }
    }
    for (j = mgr->count - 1; j >= (int)i; j--) {
        func_02023204(mgr, &mgr->items[j]);
    }
    path[n] = target;
    for (; i < n + 1; i++) {
        mgr->items[i] = path[i]->slot0();
        mgr->items[i]->owner = mgr;
        mgr->items[i]->index = i;
    }
    mgr->count = i;
}
}

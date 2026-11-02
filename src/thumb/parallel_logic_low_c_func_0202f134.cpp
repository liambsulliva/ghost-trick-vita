class Node {
public:
    virtual void v0();
    virtual void v1();
    virtual int v2();
    virtual void v3();
    Node *next;
    unsigned char flag;
    unsigned char mask;
};

struct Manager {
    unsigned char mask;
    char pad[0xb];
    Node *head;
};

extern "C" {
void func_0202f134(Manager *m)
{
    Node *n;
    n = m->head;
    if (n != 0) {
        do {
            if (n->flag != 0 && (m->mask & n->mask) != 0 && n->v2() != 0) {
                n->v3();
                return;
            }
            n = n->next;
        } while (n != 0);
    }
}
}

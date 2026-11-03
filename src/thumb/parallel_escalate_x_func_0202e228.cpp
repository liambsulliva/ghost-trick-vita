class Node {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual int find(int v, int *counter);
    unsigned char pad4[0x70];
    Node *child;
    Node *next;
};

extern "C" int func_0202e228(Node *owner, int v);

int func_0202e228(Node *owner, int v)
{
    int counter = 0;
    Node *p;
    int r;

    if (v == counter) {
        return (int)owner;
    }
    counter = counter + 1;
    p = owner->child;
    while (p != 0) {
        Node *cur = p;
        p = cur->next;
        r = cur->find(v, &counter);
        if (r != 0) {
            return r;
        }
    }
    return 0;
}

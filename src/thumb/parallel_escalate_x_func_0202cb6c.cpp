struct Node2cb6c {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c(Node2cb6c *peer, int a, int b);
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c(Node2cb6c *parent, int a, int b, int c);
    unsigned char enabled : 1;
    unsigned char shared : 1;
    char pad[0x6f];
    Node2cb6c *first;
    Node2cb6c *next;
};

extern "C" void func_0202cb6c(Node2cb6c *self, Node2cb6c *peer, int a, int b, int c)
{
    if (self->enabled) {
        self->v1c(peer, a, b);
        if (peer != 0) {
            self->shared = peer->shared;
        }
        Node2cb6c *child = self->first;
        while (child != 0) {
            Node2cb6c *cur = child;
            child = cur->next;
            cur->v3c(self, a, b, c);
        }
    }
}

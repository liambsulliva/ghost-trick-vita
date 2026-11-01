struct Node2d3d4;

struct Handler2d3d4 {
    virtual void v000();
    virtual void v004();
    virtual void v008();
    virtual void v00c();
    virtual void v010();
    virtual void v014();
    virtual void v018();
    virtual void v01c();
    virtual void v020();
    virtual void v024();
    virtual void v028();
    virtual void v02c();
    virtual void v030();
    virtual void v034();
    virtual void v038();
    virtual void v03c();
    virtual void v040();
    virtual void v044();
    virtual void v048();
    virtual void v04c();
    virtual void v050();
    virtual void v054();
    virtual void v058();
    virtual void v05c();
    virtual void v060();
    virtual void v064();
    virtual void v068();
    virtual void v06c();
    virtual void v070();
    virtual void v074();
    virtual void v078();
    virtual void v07c();
    virtual void v080();
    virtual void v084();
    virtual void v088();
    virtual void v08c();
    virtual void v090();
    virtual void v094();
    virtual void v098();
    virtual void v09c();
    virtual void v0a0();
    virtual void v0a4();
    virtual void v0a8();
    virtual void v0ac();
    virtual void v0b0();
    virtual void v0b4();
    virtual void v0b8();
    virtual void v0bc();
    virtual void v0c0();
    virtual void v0c4();
    virtual void v0c8();
    virtual void v0cc();
    virtual void v0d0();
    virtual void v0d4();
    virtual void v0d8();
    virtual void v0dc();
    virtual void v0e0();
    virtual void v0e4();
    virtual void v0e8();
    virtual void v0ec();
    virtual void v0f0();
    virtual void v0f4();
    virtual void v0f8();
    virtual void v0fc();
    virtual void v100();
    virtual void v104();
    virtual void v108();
    virtual void v10c();
    virtual int v110(Node2d3d4 *node, int a, int b, int c);
};

struct Owner2d3d4 {
    char pad[0x188];
    Handler2d3d4 *handler;
};

struct Node2d3d4 {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c(Node2d3d4 *peer, int a, int b);
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c(Node2d3d4 *parent, int a, int b, int c);
    unsigned char enabled : 1;
    char pad[0x6f];
    Node2d3d4 *first;
    Node2d3d4 *next;
    void *f7c;
    Owner2d3d4 *owner;
};

extern "C" void func_0202d3d4(Node2d3d4 *self, Node2d3d4 *peer, int a, int b, int c)
{
    if (self->enabled) {
        self->v1c(peer, a, b);
        if (self->owner->handler->v110(self, a, b, c) == 0) {
            Node2d3d4 *child = self->first;
            while (child != 0) {
                Node2d3d4 *cur = child;
                child = cur->next;
                cur->v3c(self, a, b, c);
            }
        }
    }
}

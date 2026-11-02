class Node020d3a74 {
public:
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
    virtual void v0d4(int value);
};

typedef struct Scene020d3a74 {
    char pad0[4];
    unsigned char state;
    char pad5[0x23];
    Node020d3a74 *node;
} Scene020d3a74;

extern "C" {
extern char data_ov001_020d3df0[];
extern char data_ov001_020d3e04[];
extern void func_ov001_020d3a64(Scene020d3a74 *self, int state, int a, int b, int c);
extern unsigned char *func_020312e4(void);
extern int func_0200902c(void);
extern void *func_02008444(void);
extern void func_0200fbc8(void *object);
extern int func_02027d4c(void *table, int index);
extern void *func_0202f624(int size);
extern Node020d3a74 *func_02067a84(void *object);
extern unsigned char *func_02009900(void);
extern void func_0202f7e8(void *list, Node020d3a74 *node, unsigned int a, int b);
extern void func_020521d4(void *object, int a, unsigned int b, int c);
extern void func_020303c0(void);
extern void func_ov001_020d3cf4(Scene020d3a74 *self);
extern void func_ov001_020d3b60(Scene020d3a74 *self);

void func_ov001_020d3a74(Scene020d3a74 *self)
{
    int value;
    Node020d3a74 *node;

    switch (self->state) {
    case 0:
        func_ov001_020d3a64(self, 1, 0, 0, 0);
        func_020312e4();
        if (func_0200902c() == 1) {
            func_ov001_020d3a64(self, 1, 0, 0, 0);
            return;
        }
        func_0200fbc8(func_02008444());
        value = func_02027d4c(data_ov001_020d3df0, 2);
        node = (Node020d3a74 *)func_0202f624(0xd4);
        if (node != 0) {
            node = func_02067a84(node);
        }
        self->node = node;
        node->v0d4(value);
        func_0202f7e8(func_02009900(), self->node, 1, 0);
        func_020521d4(data_ov001_020d3e04, 0, 0x1e, 0x100);
        func_020521d4(data_ov001_020d3e04, 1, 0x1e, 0x100);
        func_020303c0();
        func_ov001_020d3a64(self, 0xff, 0, 0, 0);
        break;
    case 1:
        func_ov001_020d3cf4(self);
        break;
    case 2:
        func_ov001_020d3b60(self);
        break;
    case 0xff:
        break;
    }
}
}

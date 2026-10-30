struct Root0202c038 {
    char pad[0xdc];
    int value;
};

class Obj0202c038 {
public:
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
    virtual void slot10(Obj0202c038 *object, int key, int index);
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20(int value);
    unsigned char pad04;
    unsigned char kind;
    char pad06[6];
    unsigned short key;
    char pad0e[0x62];
    Obj0202c038 *owner;
    Obj0202c038 *child;
    Obj0202c038 *next;
    Obj0202c038 *prev;
    Root0202c038 *root;
};

extern "C" {
void func_0202c038(Obj0202c038 *self)
{
    Obj0202c038 *node;
    Obj0202c038 *current;

    if ((void *)self != (void *)self->root) {
        self->slot10(self, self->key, -1);
        if (self->kind == 0x27) {
            self->slot20(self->root->value);
        }
    }
    node = self->child;
    while (node != 0) {
        current = node;
        node = node->next;
        current->slot11();
    }
}
}

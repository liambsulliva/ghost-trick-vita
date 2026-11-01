typedef struct Node2d7ac {
    void **vtable;
    unsigned char f4;
    unsigned char type;
    short pad;
    short id;
    char pad2[0x6e];
    struct Node2d7ac *next;
} Node2d7ac;

typedef struct Obj2d7ac {
    char pad[0x74];
    Node2d7ac *first;
} Obj2d7ac;

typedef Node2d7ac *(*Find2d7ac)(void *self, int key);

Node2d7ac *func_0202d7ac(Obj2d7ac *object, int key)
{
    Node2d7ac *node = object->first;
    while (node != 0) {
        Node2d7ac *cur = node;
        Node2d7ac *found;
        node = cur->next;
        if (cur->type == 0x27) {
            if (cur->id != -1 && cur->id == key) {
                return cur;
            }
            found = ((Find2d7ac)cur->vtable[0x54 / 4])(cur, key);
            if (found != 0) {
                return found;
            }
        }
    }
    return 0;
}

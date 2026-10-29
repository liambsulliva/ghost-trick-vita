typedef struct Node {
    char pad[0x70];
    struct Node *owner;
    struct Node *child;
    struct Node *next;
    struct Node *prev;
} Node;

void func_0202b53c(Node *object)
{
    Node *parent;
    int zero;

    parent = object->owner;
    if (parent != 0) {
        if (parent->child == object) {
            parent->child = object->next;
        }
    }
    if (object->prev != 0) {
        object->prev->next = object->next;
    }
    if (object->next != 0) {
        object->next->prev = object->prev;
    }
    zero = 0;
    object->owner = (Node *)zero;
    object->prev = (Node *)zero;
    object->next = (Node *)zero;
}

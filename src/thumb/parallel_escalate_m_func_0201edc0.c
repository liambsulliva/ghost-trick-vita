struct NodeList {
    char pad[0xc];
    struct Node *head;
};

struct Node {
    void *vtable;
    signed char *name;
    struct Node *next;
    int pad;
    struct NodeList *list;
    unsigned int flags : 24;
    unsigned int kind : 8;
    unsigned int hash;
};

extern char data_020a4664[];
extern struct NodeList data_020bbbb4;
extern unsigned int func_0201eee8(void *unused, signed char *text);

struct Node *func_0201edc0(struct Node *node, signed char *name, struct NodeList *list, unsigned int flags, unsigned int hash, int kind)
{
    struct Node *p;
    struct Node *next;

    node->vtable = data_020a4664;
    node->name = name;
    node->flags = flags;
    node->kind = kind;
    if (hash != 0) {
        node->hash = hash;
    } else {
        node->hash = func_0201eee8(node, name);
    }
    if (list != 0) {
        node->list = list;
    } else {
        node->list = &data_020bbbb4;
    }
    p = node->list->head;
    if (p != 0) {
        for (;;) {
            next = p->next;
            if (next == 0) {
                p->next = node;
                node->next = 0;
                break;
            }
            p = next;
        }
    } else {
        node->list->head = node;
    }
    return node;
}

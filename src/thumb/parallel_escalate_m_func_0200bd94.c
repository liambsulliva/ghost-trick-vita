typedef struct Node {
    char pad[0x13];
    signed char offset;
    char pad2[0xb];
    unsigned char level : 4;
    unsigned char rest : 4;
    char pad3[0x18];
    struct Node *next;
} Node;

typedef struct Slot {
    Node *head;
    Node *tail;
} Slot;

void func_0200bd94(char *list, Node *node)
{
    int index;
    Slot *slot;

    index = node->level + node->offset;
    if (index < 0) {
        index = 0;
    }
    if ((unsigned int)index >= 0x10) {
        index = 0xf;
    }
    slot = (Slot *)(list + 0xa00) + index;
    node->next = 0;
    if (slot->head == 0) {
        slot->head = node;
    } else {
        slot->tail->next = node;
    }
    slot->tail = node;
}

struct Node {
    unsigned char pad0[10];
    unsigned short key;
    unsigned char pad1[108];
    struct Node *next;
    struct Node *prev;
};

struct Owner {
    unsigned char pad0[116];
    struct Node *head;
};

void func_0202b4fc(struct Node *child, struct Owner *owner)
{
    struct Node *node;
    struct Node *prev;

    if (owner != 0) {
        node = owner->head;
        if (node == 0) {
            owner->head = child;
            return;
        }
        while (node != 0) {
            if (child->key < node->key) {
                prev = node->prev;
                child->prev = prev;
                if (prev != 0) {
                    prev->next = child;
                } else {
                    owner->head = child;
                }
                child->next = node;
                node->prev = child;
                return;
            }
            if (node->next == 0) {
                node->next = child;
                child->prev = node;
                return;
            }
            node = node->next;
        }
    }
    return;
}

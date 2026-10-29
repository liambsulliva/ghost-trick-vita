struct Node {
    unsigned char before[104];
    struct Node *next;
};
struct State {
    unsigned char before[36];
    struct Node *head;
};
extern struct State data_020d0f0c;

void func_02073c7c(struct Node *node)
{
    struct Node *cur;
    struct Node *prev;

    prev = 0;
    cur = data_020d0f0c.head;
    while (cur != 0 && cur != node) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == 0) {
        data_020d0f0c.head = node->next;
    }
    else {
        prev->next = node->next;
    }
}

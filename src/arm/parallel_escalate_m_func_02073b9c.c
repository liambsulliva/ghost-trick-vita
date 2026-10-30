typedef struct Node Node;
struct Node {
    char padding[0x7c];
    Node *prev;
    Node *next;
};
typedef struct {
    Node *head;
    Node *tail;
} List;

Node *func_02073b9c(List *list,Node *target)
{
    Node *node;
    Node *next;
    Node *prev;

    node = list->head;
    if (node == 0) {
        goto done;
    }
loop:
    next = node->next;
    if (node == target) {
        prev = node->prev;
        if (list->head == node) {
            list->head = next;
        } else {
            prev->next = next;
        }
        if (list->tail == node) {
            list->tail = prev;
        } else {
            next->prev = prev;
        }
        goto done;
    }
    node = next;
    if (node != 0) {
        goto loop;
    }
done:
    return node;
}

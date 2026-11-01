typedef struct node {
    unsigned int a;
    unsigned int b;
    struct node *prev;
    struct node *next;
} node;

typedef struct list {
    node *head;
    node *tail;
} list;

node *func_02068154(list *param_1, node *param_2, node *param_3)
{
    node *q;

    param_2->prev = param_3;
    if (param_3 != 0) {
        q = param_3->next;
        param_3->next = param_2;
    } else {
        q = param_1->head;
        param_1->head = param_2;
    }
    param_2->next = q;
    if (q != 0) {
        q->prev = param_2;
    } else {
        param_1->tail = param_2;
    }
    return param_2;
}

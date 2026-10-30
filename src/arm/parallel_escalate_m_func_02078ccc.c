struct Node {
    int pad0;
    int pad4;
    int priority;
    struct Node *next;
};

extern int func_02075c98(void);
extern void func_02075cac(int saved);

void func_02078ccc(struct Node **queue, struct Node *node, int priority, int mode)
{
    int saved;
    struct Node *cur;
    struct Node *prev;

    if (queue == 0) {
        return;
    }
    node->priority = priority;
    saved = func_02075c98();
    cur = *queue;
    prev = 0;
    if (cur != 0) {
        do {
            if (mode == 0) {
                if (cur->priority > priority) {
                    break;
                }
            }
            if (mode == 1) {
                if (cur->priority >= priority) {
                    break;
                }
            }
            prev = cur;
            cur = cur->next;
        } while (cur != 0);
    }
    if (cur != 0) {
        node->next = cur;
    } else {
        node->next = 0;
    }
    if (prev != 0) {
        prev->next = node;
    } else {
        *queue = node;
    }
    func_02075cac(saved);
}

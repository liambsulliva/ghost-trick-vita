extern void func_020768a8(void);
extern void func_020768bc(void);
extern void func_02074860(void *param_1, unsigned int param_2);

typedef struct Link02077368 {
    void *head;
    struct Link02077368 *next;
} Link02077368;

typedef struct {
    char pad00[0x18];
    Link02077368 links[1];
} Obj02077368;

void func_02077368(int param_1, int param_2, int param_3)
{
    int head;
    Link02077368 *link;
    Link02077368 *node;

    func_020768a8();
    head = *(int *)(param_1 + param_2 * 8 + 0x18);
    if (head != 0) {
        if (param_3 == head) {
            func_020768bc();
            return;
        }
        link = &((Obj02077368 *)param_1)->links[param_2];
        node = *(Link02077368 **)(head + 0x18);
        if (link == node) {
            *(int *)(head + 0x18) = *(int *)(param_1 + param_2 * 8 + 0x1c);
            func_02074860(*(void **)(param_1 + param_2 * 8 + 0x18), 0x3c);
        } else {
            while (node != 0) {
                if (link == node->next) {
                    break;
                }
                node = node->next;
            }
            node->next = *(Link02077368 **)((char *)param_1 + param_2 * 8 + 0x1c);
            func_02074860(node, 8);
        }
    }
    link = &((Obj02077368 *)param_1)->links[param_2];
    node = *(Link02077368 **)(param_3 + 0x18);
    *(Link02077368 **)(param_3 + 0x18) = link;
    *(Link02077368 **)((char *)param_1 + param_2 * 8 + 0x1c) = node;
    *(int *)((char *)param_1 + param_2 * 8 + 0x18) = param_3;
    func_020768bc();
    func_02074860((void *)param_1, 0x3c);
    func_02074860((void *)param_3, 0x3c);
}

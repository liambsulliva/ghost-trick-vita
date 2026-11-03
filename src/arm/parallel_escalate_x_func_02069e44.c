struct N { struct N *next; int arg; void (*func)(int arg, int a, int b); };
struct N *data_020d07c0 = 0;
extern int func_02075c98(void);
extern void func_02075cac(int e);
void func_02069e44(int a, int b)
{
    int e;
    struct N **link;
    struct N *node;

    e = func_02075c98();
    node = data_020d07c0;
    link = &data_020d07c0;
    while (node != 0) {
        if (node->func != 0) {
            node->func(node->arg, a, b);
        }
        if (*link == node) {
            link = (struct N **)*link;
        }
        node = *link;
    }
    func_02075cac(e);
}

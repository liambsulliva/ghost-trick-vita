struct Node {
    unsigned char pad[0x54];
    struct Node *next;
};
struct Walker {
    unsigned char pad0[0x3c];
    struct Node *head;
    struct Node *cur;
    struct Node *fallback;
    unsigned char pad1[4];
    unsigned char stop;
};
extern int func_02008444();
extern int func_0200fe58();
extern int func_020509c4();
extern int func_02050f8c();
extern void func_02050f94();

void func_02050f24(struct Walker *walker)
{
    int flag;
    struct Node *node;

    flag = 0;
    walker->cur = walker->head;
    while (walker->cur != 0) {
        if (func_02050f8c() != 0) {
            flag = 1;
        }
        func_020509c4(walker->cur);
        if (walker->stop != 0) {
            return;
        }
        walker->cur = walker->cur->next;
    }
    if (flag == 0) {
        func_02008444();
        if (func_0200fe58() != 0) {
            node = walker->fallback;
            if (node != 0) {
                walker->cur = node;
                func_02050f94(node,1);
                func_020509c4(walker->cur);
                func_02050f94(walker->cur,0);
            }
        }
    }
    walker->cur = 0;
}

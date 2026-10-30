class Heap0202b41c {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int size, char *name);
    virtual void slot3();
    virtual void slot4(void *object);
};

extern "C" {
extern char data_02085298;
extern void *func_02007bd8(void);
extern Heap0202b41c *func_02007b70(void *object);
}

class Node0202b41c {
public:
    char pad[0x278];
    int value;
    Node0202b41c *next;
};

struct List0202b41c {
    Node0202b41c *cursor;
    Node0202b41c *head;
};

extern "C" {
extern struct List0202b41c data_020ca1d8;

void func_0202b41c(void)
{
    Node0202b41c *node;
    Node0202b41c *next;

    if (data_020ca1d8.head == 0) {
        data_020ca1d8.cursor = data_020ca1d8.head = (Node0202b41c *)func_02007b70(func_02007bd8())->slot2(0x280, &data_02085298);
        data_020ca1d8.cursor->value = 0;
        data_020ca1d8.cursor->next = 0;
        return;
    }
    node = data_020ca1d8.head->next;
    data_020ca1d8.cursor = data_020ca1d8.head;
    while (node != 0) {
        next = node->next;
        if (node != 0) {
            if (func_02007b70(func_02007bd8()) != 0) {
                func_02007b70(func_02007bd8())->slot4(node);
            }
        }
        node = next;
    }
    data_020ca1d8.cursor->value = 0;
    data_020ca1d8.cursor->next = 0;
}
}

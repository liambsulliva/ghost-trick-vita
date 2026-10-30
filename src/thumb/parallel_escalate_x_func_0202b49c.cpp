class Pool0202b49c {
public:
    virtual void v00();
    virtual void v04();
    virtual void *v08(int size, void *owner);
};

struct Node0202b49c {
    char pad[0x278];
    struct Node0202b49c *next;
    int busy;
};

struct List0202b49c {
    struct Node0202b49c *cur;
    struct Node0202b49c *end;
};

extern "C" {
extern struct List0202b49c data_020ca1d8;
extern char data_02085298;
void *func_02007bd8(void);
Pool0202b49c *func_02007b70(void *object);

void *func_0202b49c(int size)
{
    struct Node0202b49c *cur;
    struct Node0202b49c *next;

    cur = data_020ca1d8.cur;
    if (data_020ca1d8.end == cur) {
        return func_02007b70(func_02007bd8())->v08(0x280, &data_02085298);
    }
    next = cur->next;
    next->busy = 0;
    data_020ca1d8.cur = next;
    return cur;
}
}

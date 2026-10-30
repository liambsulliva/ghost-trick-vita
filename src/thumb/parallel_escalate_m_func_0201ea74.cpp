class Res0201ea74 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int size, char *name);
};

typedef struct Node0201ea74 {
    int value;
    struct Node0201ea74 *next;
    struct Node0201ea74 *prev;
} Node0201ea74;

typedef struct {
    int first;
    Node0201ea74 node;
    char pad[0x5f4];
    Node0201ea74 *cursor;
    int zero;
    int buffer;
    int count;
} Self0201ea74;

extern "C" {
extern char data_02084e08;
extern void *func_02007bd8(void);
extern Res0201ea74 *func_02007b70(void *object);

Self0201ea74 *func_0201ea74(Self0201ea74 *self, int first, int unused, int count)
{
    self->first = first;
    self->count = count;
    self->buffer = func_02007b70(func_02007bd8())->slot2(count * 8, &data_02084e08);
    self->zero = 0;
    self->cursor = &self->node;
    self->cursor->value = 0;
    self->cursor->prev = 0;
    self->cursor->next = 0;
    return self;
}
}

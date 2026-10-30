struct Buffer {
    void *vtable;
    int buffer;
    int size;
    int stride;
    int count;
    char pad[0x20];
    unsigned char mode;
};

extern char data_020a4520[];
extern void func_0201dea0(struct Buffer *self);

struct Buffer *func_0201ddb4(struct Buffer *self, int count, unsigned char mode)
{
    self->vtable = data_020a4520;
    self->buffer = 0;
    self->size = 0;
    self->stride = 0;
    self->count = count;
    self->mode = mode;
    func_0201dea0(self);
    return self;
}

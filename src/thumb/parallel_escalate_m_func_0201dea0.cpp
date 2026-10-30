class Res0201dea0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int size, char *name);
};

class Self0201dea0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    int buffer;
    int size;
    int stride;
    int count;
    char pad[0x20];
    unsigned char mode;
};

extern "C" {
extern char data_02084dcc;
extern void *func_02007bd8(void);
extern Res0201dea0 *func_02007b70(void *object);
extern void func_0201defc(Self0201dea0 *object);

void func_0201dea0(Self0201dea0 *self)
{
    int size;

    if (self->mode == 3) {
        self->stride = 0xc000;
        self->size = self->count * 0xc000;
    } else if (self->mode == 2) {
        self->stride = 0x6000;
        self->size = self->count * 0x6000;
    }
    size = self->size;
    if (size != 0) {
        self->buffer = func_02007b70(func_02007bd8())->slot2(size, &data_02084dcc);
        func_0201defc(self);
        self->slot2();
    }
}
}

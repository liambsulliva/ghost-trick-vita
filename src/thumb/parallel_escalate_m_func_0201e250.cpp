class Self0201e250 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int value, char *name);
    char pad[0x2c];
    char *child;
    int extra;
};

extern "C" {
extern char data_02084dd8;
extern char data_02084ddc;
extern char data_02084de8;
extern char *func_020312bc(unsigned int size, char *name);
extern char *func_0200c504(char *object);
extern void func_0201e144(char *object, int resource, int value, unsigned int mode);
extern void func_0200c530(char *object, char *name);
extern void *func_02007bd8(void);
extern void func_0201f288(void *object, char *value);

void func_0201e250(Self0201e250 *self, int value, int extra)
{
    int resource;
    char *child;

    resource = self->slot2(value, &data_02084dd8);
    if (resource == 0) {
        return;
    }
    child = func_020312bc(0x3c, &data_02084ddc);
    if (child != 0) {
        child = func_0200c504(child);
    }
    self->child = child;
    if (child == 0) {
        return;
    }
    if (value < 0) {
        value = -value;
    }
    func_0201e144(child, resource, value, 4);
    func_0200c530(self->child, &data_02084de8);
    self->extra = extra;
    *(unsigned char *)(self->child + 0x38) = 1;
    func_0201f288(func_02007bd8(), self->child);
}
}

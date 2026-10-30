extern "C" {
extern char data_020a4e24;
extern void func_02028b28(void *object);
extern void func_02031270(void *object);
extern void func_020293f8(void *object);
extern void func_0202f644(void *object);
}

class Child02029480 {
public:
    ~Child02029480() { func_02028b28(this); }
    static void operator delete(void *object) { func_02031270(object); }
};

typedef struct {
    char *vtable;
    char pad[0x44];
    Child02029480 *child;
} Self02029480;

extern "C" {
Self02029480 *func_02029480(Self02029480 *self)
{
    self->vtable = &data_020a4e24;
    if (self->child != 0) {
        delete self->child;
        self->child = 0;
    }
    func_020293f8(self);
    func_0202f644(self);
    return self;
}
}

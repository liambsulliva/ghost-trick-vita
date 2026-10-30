extern "C" {
extern char data_020a4e24;
extern void func_02028b28(void *object);
extern void func_02031270(void *object);
extern void func_020293f8(void *object);
}

class Child020294b4 {
public:
    ~Child020294b4() { func_02028b28(this); }
    static void operator delete(void *object) { func_02031270(object); }
};

typedef struct {
    char *vtable;
    char pad[0x44];
    Child020294b4 *child;
} Self020294b4;

extern "C" {
Self020294b4 *func_020294b4(Self020294b4 *self)
{
    self->vtable = &data_020a4e24;
    if (self->child != 0) {
        delete self->child;
        self->child = 0;
    }
    func_020293f8(self);
    return self;
}
}

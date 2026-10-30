class Child0203404c {
public:
    virtual ~Child0203404c();
};

struct Self0203404c {
    void *table;
    char pad04[0xc8];
    Child0203404c *child;
};

extern "C" {
extern char data_020a5a14;
extern void func_ov006_020d4d28(void *object);
extern void *func_02056cac(void *object);
extern void func_0202f644(void *object);

struct Self0203404c *func_0203404c(struct Self0203404c *self)
{
    self->table = &data_020a5a14;
    func_ov006_020d4d28(self);
    if (self->child != 0) {
        delete self->child;
        self->child = 0;
    }
    func_02056cac(self);
    func_0202f644(self);
    return self;
}
}

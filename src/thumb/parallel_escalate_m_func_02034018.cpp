class Child02034018 {
public:
    virtual ~Child02034018();
};

struct Self02034018 {
    void *table;
    char pad04[0xc8];
    Child02034018 *child;
};

extern "C" {
extern char data_020a5a14;
extern void func_ov006_020d4d28(void *object);
extern void *func_02056cac(void *object);

struct Self02034018 *func_02034018(struct Self02034018 *self)
{
    self->table = &data_020a5a14;
    func_ov006_020d4d28(self);
    if (self->child != 0) {
        delete self->child;
        self->child = 0;
    }
    func_02056cac(self);
    return self;
}
}

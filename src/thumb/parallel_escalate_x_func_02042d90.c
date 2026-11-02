struct Entry2d90 {
    int a;
    int b;
    int c;
    unsigned short d;
    short e;
};

struct Child2d90 {
    void (**vtable)(struct Child2d90 *self);
};

struct View2d90 {
    void *vtable;
    char pad04[0x24];
    int owner;
    unsigned char mode;
    unsigned char count;
    unsigned short timer;
    unsigned char flag;
    char pad31[3];
    int w34;
    void *sprite;
    void *top;
    void *bottom;
    int w44;
    struct Entry2d90 entries[5];
    int delay;
    unsigned short h9c;
    unsigned short h9e;
    unsigned short ha0;
    unsigned short ha2;
    unsigned char ba4;
};

extern char data_020a7e94;
extern char data_020cece8;
extern char data_020ce820;
extern void func_0202f524(struct View2d90 *self);
extern void func_0202f7e8(void *manager, void *object, int a, int b);
extern void *func_02009900(void);
extern void *func_0202f624(int size);
extern void *func_02044170(void *object, int kind);
extern void func_02044354(void *object, int x, int y, int z, int w);
extern void func_0202fbb0(void *iter);
extern int func_0200eac4(void *iter);
extern void *func_0200eab4(void *iter);
extern int func_02008828(void *object, void *type);
extern struct Child2d90 *func_0200eab8(void *iter);
extern void func_0200eabc(void *iter, int step);
extern void func_02043a80(struct View2d90 *self, int value);
extern void func_02043b8c(struct View2d90 *self, int value);
extern void *func_0204ed1c(void *data);

struct View2d90 *func_02042d90(struct View2d90 *self, int owner)
{
    unsigned int i;
    void *sprite;
    void *part;
    unsigned char iter[16];
    struct Child2d90 *child;

    func_0202f524(self);
    self->vtable = &data_020a7e94;
    self->mode = 0;
    self->count = 3;
    self->owner = owner;
    self->timer = 0;
    self->flag = 0;
    self->w34 = 0;
    self->sprite = 0;
    self->top = 0;
    self->bottom = 0;
    self->w44 = 0;
    for (i = 0; i < 5; i++) {
        self->entries[i].a = 0;
        self->entries[i].b = 0;
        self->entries[i].c = 0;
        self->entries[i].d = i;
        self->entries[i].e = i;
    }
    sprite = func_0204ed1c(&data_020cece8);
    self->sprite = sprite;
    if (sprite != 0) {
        func_0202f7e8(func_02009900(), sprite, 1, 0);
    }
    part = func_0202f624(0xe8);
    if (part != 0) {
        part = func_02044170(part, 1);
    }
    self->top = part;
    if (part != 0) {
        func_02044354(part, 0x77000, 0x3c000, 0x8c000, 0x3c000);
        func_0202f7e8(func_02009900(), self->top, 1, 0);
    }
    part = func_0202f624(0xe8);
    if (part != 0) {
        part = func_02044170(part, 2);
    }
    self->bottom = part;
    if (part != 0) {
        func_02044354(part, 0x77000, -0x54000, 0x8c000, -0x54000);
        func_0202f7e8(func_02009900(), self->bottom, 1, 0);
    }
    func_0202fbb0(iter);
    while (func_0200eac4(iter) != 0) {
        if (func_02008828(func_0200eab4(iter), &data_020ce820) != 0) {
            child = func_0200eab8(iter);
            child->vtable[11](child);
        }
        func_0200eabc(iter, 0);
    }
    func_02043a80(self, 1);
    func_02043b8c(self, 0);
    self->delay = 0x3c;
    self->h9c = 0;
    self->h9e = 0;
    self->ha0 = 0xc;
    self->ha2 = 0;
    self->ba4 = 0;
    return self;
}

struct Slot42ef0 {
    int a;
    int b;
    int c;
    int d;
};
struct Self42ef0 {
    void *vtable;
    char pad04[0x34];
    int w38;
    int w3c;
    int w40;
    int w44;
    struct Slot42ef0 slots[5];
};
struct Iter42ef0 {
    int words[4];
};
struct VObj42ef0 {
    void (**vtable)(struct VObj42ef0 *obj);
};
extern char data_020a7e94[];
extern char data_020ce820[];
extern void func_0202f64c(void *item);
extern void func_0202fbb0(struct Iter42ef0 *it);
extern int func_0200eac4(struct Iter42ef0 *it);
extern void *func_0200eab4(struct Iter42ef0 *it);
extern int func_02008828(void *item,void *key);
extern struct VObj42ef0 *func_0200eab8(struct Iter42ef0 *it);
extern void func_0200eabc(struct Iter42ef0 *it,int a);
extern void func_02043a80(struct Self42ef0 *self,int a);
extern void func_02043b8c(struct Self42ef0 *self,int a);
extern void func_0202f5e4(struct Self42ef0 *self);
struct Self42ef0 *func_02042ef0(struct Self42ef0 *self)
{
    unsigned int i;
    struct Iter42ef0 it;
    struct VObj42ef0 *obj;
    self->vtable = data_020a7e94;
    for (i = 0; i < 5; i++) {
        func_0202f64c(&self->slots[i].a);
        func_0202f64c(&self->slots[i].b);
        func_0202f64c(&self->slots[i].c);
    }
    func_0202f64c(&self->w3c);
    func_0202f64c(&self->w40);
    func_0202f64c(&self->w38);
    func_0202f64c(&self->w44);
    func_0202fbb0(&it);
    while (func_0200eac4(&it) != 0) {
        if (func_02008828(func_0200eab4(&it),data_020ce820) != 0) {
            obj = func_0200eab8(&it);
            obj->vtable[10](obj);
        }
        func_0200eabc(&it,0);
    }
    func_02043a80(self,0);
    func_02043b8c(self,1);
    func_0202f5e4(self);
    return self;
}

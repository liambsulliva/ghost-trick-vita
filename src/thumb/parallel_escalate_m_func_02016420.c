struct Counter {
    int first;
    int count;
};
struct Object {
    void *vtable;
    int base[9];
    int a;
    int b;
    int c;
    int d;
    int e;
};
extern char data_020a35dc[];
extern struct Counter data_020b9d14;
extern void func_0202f524(struct Object *object);

struct Object *func_02016420(struct Object *object)
{
    func_0202f524(object);
    object->vtable = data_020a35dc;
    object->a = 0;
    object->b = 0;
    object->c = 0;
    object->d = 0;
    object->e = 1;
    data_020b9d14.count = data_020b9d14.count + 1;
    return object;
}

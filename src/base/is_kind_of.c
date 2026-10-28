struct Object;

struct ObjectMethods {
    void *slots[4];
    void *(*get_type)(struct Object *object);
};

struct Object {
    struct ObjectMethods *methods;
};

int func_02008314(void *type, void *kind);

int func_02008828(struct Object *object, void *kind)
{
    return func_02008314(object->methods->get_type(object), kind);
}

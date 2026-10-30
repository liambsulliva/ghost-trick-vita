struct Flags {
    unsigned int low : 7;
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int high : 22;
};
struct Object {
    int pad[3];
    struct Flags flags;
};
extern int func_02010ccc(struct Object *object);

void func_02010cdc(struct Object *object)
{
    if (func_02010ccc(object) != 0) {
        return;
    }
    object->flags.b = 0;
    object->flags.a = 1;
    object->flags.c = 1;
}

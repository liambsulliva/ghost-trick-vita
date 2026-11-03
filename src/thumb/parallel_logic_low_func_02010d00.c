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

void func_02010d00(struct Object *object)
{
    object->flags.b = 0;
    object->flags.a = 1;
    object->flags.c = 1;
}

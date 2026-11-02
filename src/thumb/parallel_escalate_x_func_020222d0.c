struct Object;

struct Registry {
    int count;
    struct Object **objects;
};

int func_020222d0(struct Registry *registry, struct Object *object)
{
    int i;
    int count;

    i = 0;
    count = registry->count;
    for (; i < count; i++) {
        if (registry->objects[i] == object) {
            return i;
        }
    }
    return -1;
}

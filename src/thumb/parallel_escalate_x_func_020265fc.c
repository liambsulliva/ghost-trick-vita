struct Info {
    unsigned int kind : 8;
    unsigned int rest : 24;
};

struct Object {
    char pad[0x10];
    struct Info info;
};

unsigned char func_020265fc(struct Object *object)
{
    return object->info.kind;
}

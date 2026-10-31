struct Entry0201d6d4 {
    int a;
    int b;
    int c;
    short d;
    unsigned short id;
    short f;
};

struct Object0201d6d4 {
    int f00;
    int f04;
    int f08;
    int f0c;
    int f10;
    short f14;
    unsigned short f16;
    unsigned char f18;
    unsigned char f19;
    unsigned char buffer[0x1d4c];
    struct Entry0201d6d4 entries[30];
};

extern void func_0201d6c8(void *object, int value);

struct Object0201d6d4 *func_0201d6d4(struct Object0201d6d4 *object)
{
    unsigned int i;

    object->f16 = 0xffff;
    object->f00 = 0;
    object->f04 = 0;
    object->f08 = 0;
    object->f14 = 0;
    object->f18 = 0;
    object->f19 = 0;
    func_0201d6c8(object->buffer, sizeof(object->buffer));
    for (i = 0; i < 30; i++) {
        object->entries[i].a = 0;
        object->entries[i].b = 0;
        object->entries[i].c = 0;
        object->entries[i].d = 0;
        object->entries[i].id = 0xffff;
        object->entries[i].f = 0;
    }
    object->f0c = 0;
    object->f10 = 0;
    return object;
}

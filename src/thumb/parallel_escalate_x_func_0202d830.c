typedef struct Obj2d830 {
    char pad[0x84];
    unsigned short count;
    unsigned char flags;
    unsigned char f87;
    int current;
    char pad2[0x30];
    int *offsets;
    char *base;
} Obj2d830;

void func_0202d830(Obj2d830 *obj, int key)
{
    int i;
    short value;
    for (i = 0; i < obj->count; i = (short)(i + 1)) {
        value = *(short *)(obj->base + obj->offsets[i]);
        if (value == key) {
            obj->current = i;
            obj->f87 = 0;
            break;
        }
    }
    obj->flags |= 2;
}

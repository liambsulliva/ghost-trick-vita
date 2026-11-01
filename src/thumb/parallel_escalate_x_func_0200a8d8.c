typedef struct Obj0a8d8 {
    char pad[0x10];
    unsigned char busy;
    char pad2[0x17];
    unsigned char *data;
    int f2c;
    int f30;
    int f34;
    int f38;
} Obj0a8d8;

void func_0200a8d8(Obj0a8d8 *object)
{
    object->f2c = 0;
    object->f30 = object->data[0xaa3f] - 1;
    object->f34 = 1;
    object->f38 = 0;
    object->busy = 0;
}

typedef struct Obj0a8c8 {
    char pad[5];
    unsigned char flags;
    short pad2;
    short x;
    short y;
} Obj0a8c8;

void func_0200a8c8(Obj0a8c8 *object, short x, short y)
{
    object->flags |= 2;
    object->x = x;
    object->y = y;
}

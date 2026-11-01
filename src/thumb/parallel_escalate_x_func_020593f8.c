typedef struct Obj593f8 {
    char pad[0xd8];
    void *target;
    unsigned char x;
    unsigned char y;
    unsigned char active : 1;
} Obj593f8;

void func_020593f8(Obj593f8 *object, void *target, unsigned char x, unsigned char y)
{
    object->target = target;
    object->x = x;
    object->y = y;
    object->active = target != 0;
}

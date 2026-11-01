typedef int (*Method167e8)(void *self, int arg);

typedef struct Obj167e8 {
    char pad[0xc];
    void ***child;
} Obj167e8;

int func_020167e8(Obj167e8 *object, int arg)
{
    void ***child = object->child;
    if (child != 0) {
        return ((Method167e8)(*child)[0x58 / 4])(child, arg);
    }
    return 0;
}

typedef int (*Method65fc0)(void *self, int arg);

typedef struct Obj65fc0 {
    char pad[0xc8];
    void ***child;
} Obj65fc0;

int func_02065fc0(Obj65fc0 *object, int arg)
{
    void ***child = object->child;
    if (child != 0) {
        return ((Method65fc0)(*child)[0xa8 / 4])(child, arg);
    }
    return 0;
}

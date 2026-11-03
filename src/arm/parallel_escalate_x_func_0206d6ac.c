typedef int (*Op0206d6ac)(void *self, int a, int b, int c);

typedef struct Inner0206d6ac {
    char pad[0x20];
    Op0206d6ac *ops;
} Inner0206d6ac;

typedef struct Obj0206d6ac {
    int f0;
    int f4;
    Inner0206d6ac *inner;
    char pad[0x20];
    int pos;
    int arg;
    int f34;
    int step;
} Obj0206d6ac;

void func_0206d6ac(Obj0206d6ac *object, int p2, int p3, int p4)
{
    Inner0206d6ac *inner = object->inner;
    Op0206d6ac *ops = inner->ops;
    int arg = object->arg;
    int pos = object->pos;
    int step = object->step;
    object->pos = pos + step;
    ops[8](inner, arg, pos, step);
}

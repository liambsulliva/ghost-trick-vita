typedef int (*Op0206d6dc)(void *self, int a, int b, int c);

typedef struct Inner0206d6dc {
    char pad[0x20];
    Op0206d6dc *ops;
} Inner0206d6dc;

typedef struct Obj0206d6dc {
    int f0;
    int f4;
    Inner0206d6dc *inner;
    char pad[0x20];
    int pos;
    int arg;
    int f34;
    int step;
} Obj0206d6dc;

void func_0206d6dc(Obj0206d6dc *object, int p2, int p3, int p4)
{
    Inner0206d6dc *inner = object->inner;
    Op0206d6dc *ops = inner->ops;
    int arg = object->arg;
    int pos = object->pos;
    int step = object->step;
    object->pos = pos + step;
    ops[9](inner, arg, pos, step);
}

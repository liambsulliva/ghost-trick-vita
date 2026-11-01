typedef struct Pair16b6c {
    unsigned short x;
    unsigned short y;
} Pair16b6c;

typedef Pair16b6c (*Method16b6c)(void *self, int a, int b);

typedef struct Obj16b6c {
    int f0;
    void ***child;
} Obj16b6c;

extern Pair16b6c data_02084a34;

Pair16b6c func_02016b6c(Obj16b6c *object, int a, int b)
{
    void ***child = object->child;
    if (child != 0) {
        return ((Method16b6c)(*child)[0xa8 / 4])(child, a, b);
    }
    return data_02084a34;
}

typedef struct Size0e03c {
    unsigned char w;
    unsigned char h;
    char pad[14];
} Size0e03c;

typedef struct Obj0e03c {
    int f0;
    int kind;
    int f8;
    int mode;
    int area;
} Obj0e03c;

extern Size0e03c data_02083c78[];

void func_0200e03c(Obj0e03c *object, int mode)
{
    int kind = object->kind;
    if (kind != 4 && (unsigned int)(mode - 2) <= 1) {
        object->mode = mode;
        if (mode == 2) {
            object->area = (data_02083c78[kind].w * data_02083c78[kind].h) / 2;
        } else if (mode == 3) {
            object->area = data_02083c78[kind].w * data_02083c78[kind].h;
        }
    }
}

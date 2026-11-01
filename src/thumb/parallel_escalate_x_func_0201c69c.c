typedef struct Obj1c69c {
    int f0;
    int target;
    char pad[0x12];
    unsigned short kind;
    unsigned char b1c;
    unsigned char b1d;
    unsigned short h1e;
    unsigned short h20;
    char pad2[4];
    unsigned short h26;
    unsigned short width;
    unsigned short height;
    char pad3[9];
    unsigned char b35;
} Obj1c69c;

extern void func_0201c760(Obj1c69c *object);

void func_0201c69c(Obj1c69c *object, int target, unsigned short width, unsigned short height, unsigned char kind)
{
    object->target = target;
    object->width = width;
    object->height = height;
    object->kind = kind;
    object->b35 = 0;
    object->h1e = 0;
    object->h20 = 0;
    object->b1d = 0;
    object->b1c = 0;
    object->h26 = 0;
    func_0201c760(object);
}

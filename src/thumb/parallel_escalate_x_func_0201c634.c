typedef struct Mark1c634 {
    unsigned short id;
    unsigned short value;
} Mark1c634;

typedef struct Obj1c634 {
    int f0;
    int f4;
    int owner;
    int fc;
    unsigned short x;
    unsigned short y;
    int extra;
    char pad[0x14];
    unsigned short x2;
    unsigned short y2;
    char pad2[4];
    unsigned char count;
    char pad3[0x1ccf];
    Mark1c634 marks[1];
} Obj1c634;

void func_0201c634(Obj1c634 *object, int owner, unsigned short x, unsigned short y, int extra)
{
    unsigned int i;
    object->x = x;
    object->x2 = x;
    object->extra = extra;
    object->owner = owner;
    object->y = y;
    object->y2 = y;
    for (i = 0; i < object->count; i++) {
        object->marks[i].id = 0xffff;
        object->marks[i].value = 0;
    }
    object->count = 0;
}

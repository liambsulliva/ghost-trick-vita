typedef struct Point2abf8 {
    short x;
    short y;
    int pad;
} Point2abf8;

typedef struct Item2abf8 {
    int kind;
    int f4;
    int f8;
} Item2abf8;

typedef struct Obj2abf8 {
    char pad[0x24];
    unsigned char hidden;
    char pad2[0x1c7];
    Item2abf8 items[1];
    char pad3[0x50];
    short x;
    short y;
} Obj2abf8;

extern Point2abf8 data_02080e84[];
extern void func_0202b074(Obj2abf8 *obj, int *x, int *y, int a, int b);
extern void func_02007330(int a, int b, int c, int d, int e, int f, int g, int h, short i, signed char j, int k, int l);

void func_0202abf8(Obj2abf8 *obj, int index, int a, int b)
{
    int x;
    int y;
    if (obj->hidden == 1) {
        return;
    }
    func_0202b074(obj, &x, &y, a, b);
    func_02007330(-1, data_02080e84[obj->items[index].kind].y, data_02080e84[obj->items[index].kind].x, 0, 0x2800, 0, 0, 0, x >> 1, y, 0, index);
    obj->x = x;
    obj->y = y;
}

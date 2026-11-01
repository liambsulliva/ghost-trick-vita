typedef struct Obj2e184 {
    char pad[0x174];
    int first;
    int second;
    char pad2[4];
    unsigned short base;
    char pad3[10];
    unsigned char f0 : 1;
    unsigned char f1 : 1;
    unsigned char f2 : 1;
    unsigned char hidden : 1;
    unsigned char forced : 1;
} Obj2e184;

extern void func_0202d3d4(Obj2e184 *object, int mode, int first, int second, unsigned short *value);
extern void func_0202e284(Obj2e184 *object);

void func_0202e184(Obj2e184 *object)
{
    unsigned short value;
    if (!object->hidden || object->forced) {
        value = object->base + 15;
        func_0202d3d4(object, 0, object->first, object->second, &value);
        func_0202e284(object);
    }
}

typedef struct Obj09e84 {
    char pad[5];
    unsigned char first : 1;
    unsigned char second : 1;
    unsigned char third : 1;
} Obj09e84;

void func_02009e84(Obj09e84 *object, unsigned char value)
{
    object->third = value;
}

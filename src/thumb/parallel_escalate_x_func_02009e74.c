typedef struct Obj09e74 {
    char pad[5];
    unsigned char first : 1;
    unsigned char second : 1;
    unsigned char third : 1;
} Obj09e74;

void func_02009e74(Obj09e74 *object, unsigned char value)
{
    object->first = value;
}

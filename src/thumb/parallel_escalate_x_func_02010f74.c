typedef struct Obj10f74 {
    char pad[0x2a];
    unsigned char ready;
    unsigned char pad2;
    signed char index;
} Obj10f74;

void func_02010f74(Obj10f74 *object)
{
    object->ready = 1;
    object->index = -1;
}

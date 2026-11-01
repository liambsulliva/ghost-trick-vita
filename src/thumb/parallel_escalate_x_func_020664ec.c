typedef struct Obj664ec {
    char pad[0x8c];
    unsigned char flag;
    unsigned char pad2;
    unsigned short value;
    unsigned short count;
} Obj664ec;

void func_020664ec(Obj664ec *object, unsigned short value)
{
    object->value = value;
    object->count = 0;
    object->flag = 0;
}

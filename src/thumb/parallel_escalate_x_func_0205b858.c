typedef struct Obj5b858 {
    char pad[0x9a];
    short column;
    signed char row;
} Obj5b858;

int func_0205b858(Obj5b858 *object)
{
    int offset = (1 - object->row) * 42;
    return offset + object->column;
}

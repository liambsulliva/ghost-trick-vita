typedef struct Grid2d8f8 {
    char pad[0x17c];
    unsigned short cell;
    unsigned short columns;
} Grid2d8f8;

typedef struct Obj2d8f8 {
    char pad[0x80];
    Grid2d8f8 *grid;
    char pad2[3];
    unsigned char x;
    unsigned int row;
} Obj2d8f8;

void func_0202d8f8(Obj2d8f8 *object, int index)
{
    object->row = index / object->grid->columns;
    object->x = object->grid->cell * (index % object->grid->columns);
}

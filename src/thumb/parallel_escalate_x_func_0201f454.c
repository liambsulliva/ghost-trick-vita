typedef struct Pos1f454 {
    int f0;
    unsigned int pos;
    unsigned int limit;
} Pos1f454;

unsigned int func_0201f454(Pos1f454 *object, unsigned int value, int mode)
{
    switch (mode) {
    case 0:
        object->pos = value;
        break;
    case 2:
        object->pos = object->limit - value;
        break;
    case 1:
        object->pos = object->pos + value;
        break;
    }
    if (object->limit < object->pos) {
        object->pos = object->limit;
    }
    return object->pos;
}

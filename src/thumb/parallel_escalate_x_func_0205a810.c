typedef struct Obj5a810 {
    char pad[0x10];
    unsigned char state;
    char pad2[0x1b];
    int pos;
    char pad3[0x98];
    int target;
} Obj5a810;

void func_0205a810(Obj5a810 *object)
{
    unsigned char state = object->state;
    switch (state) {
    case 0:
        object->target = object->pos;
        object->pos = object->pos - 0x18000;
        object->state = state + 1;
        break;
    case 1:
        if (object->pos < object->target) {
            object->pos = object->pos + 0x2000;
        } else {
            object->pos = object->target;
            object->state = state + 1;
        }
        break;
    }
}

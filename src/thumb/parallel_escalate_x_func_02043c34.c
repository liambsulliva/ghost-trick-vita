typedef struct Mover43c34 {
    char pad[0x10];
    unsigned char state;
    char pad2[2];
    unsigned char delay;
    char pad3[0x18];
    int cur;
    char pad4[0x98];
    int target;
    int step;
} Mover43c34;

void func_02043c34(Mover43c34 *obj)
{

    if (obj->delay != 0) {
        obj->delay--;
        return;
    }
    if (obj->state != 2) {
        return;
    }
    if (obj->target > obj->cur) {
        obj->cur = obj->cur + obj->step;
        if (obj->target < obj->cur) {
            obj->cur = obj->target;
            obj->state = 1;
        }
    } else if (obj->target < obj->cur) {
        obj->cur = obj->cur - obj->step;
        if (obj->target > obj->cur) {
            obj->cur = obj->target;
            obj->state = 1;
        }
    } else {
        obj->cur = obj->target;
        obj->state = 1;
    }
}

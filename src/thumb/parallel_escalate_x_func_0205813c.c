typedef struct Obj5813c {
    char pad[0xd5];
    unsigned char start;
    unsigned char current;
    unsigned char goal;
    signed char step;
    unsigned char done : 1;
    unsigned char rest : 7;
    unsigned char speed;
} Obj5813c;

void func_0205813c(Obj5813c *obj, unsigned char current, unsigned char goal, unsigned char speed)
{
    obj->current = current;
    obj->start = current;
    obj->goal = goal;
    obj->done = 0;
    obj->speed = speed;
    obj->step = current - goal;
    if (obj->step < 0) {
        obj->step *= -1;
    }
    if (obj->step == 0) {
        obj->step = 1;
    }
}

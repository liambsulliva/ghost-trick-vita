struct Self020380e8 {
    void *object;
    unsigned short command;
};

extern int func_02038004(void *object, int direction);
extern int func_02038064(void *object);
extern void func_02038090(void *object, unsigned char value);

int func_020380e8(struct Self020380e8 *self)
{
    void *object;
    int direction;
    int result;

    object = self->object;
    direction = 0;
    result = 0;
    if (object != 0 && self->command != 0) {
        switch (self->command) {
        case 1:
            direction = -1;
            result = func_02038004(object, direction);
            break;
        case 2:
            direction = 1;
            result = func_02038004(object, direction);
            break;
        case 3:
            direction = -2;
            result = func_02038004(object, direction);
            break;
        case 4:
            direction = 2;
            result = func_02038004(object, direction);
            break;
        case 5:
            result = func_02038004(object, direction);
            break;
        case 6:
            result = func_02038064(object);
            break;
        case 8:
            result = 1;
            break;
        case 9:
            result = 1;
            break;
        case 10:
            result = 1;
            func_02038090(object, direction);
            break;
        case 11:
            result = 1;
            func_02038090(object, 1);
            break;
        }
    }
    self->command = 0;
    return result;
}

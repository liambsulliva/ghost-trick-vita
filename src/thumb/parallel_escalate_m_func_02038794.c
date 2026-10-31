struct State02038794 {
    char before[0x26];
    short phase;
    int mode;
    int count;
    signed char dir;
    unsigned char ready;
    char middle[3];
    unsigned char busy;
};

extern void func_02037aa4(int value);

int func_02038794(struct State02038794 *object)
{
    int count;

    func_02037aa4(5);
    object->mode = 3;
    object->busy = 1;
    count = object->count;
    object->count = 0;
    object->ready = 1;
    object->phase = 2;
    if (count > 0) {
        if (count < 2) {
            object->dir = 0;
            return -count;
        }
        object->dir = 1;
        return 4 - count;
    } else if (count < 0) {
        if (count > -2) {
            object->dir = 0;
            return -count;
        }
        object->dir = -1;
        return -4 - count;
    } else {
        object->busy = 0;
        object->dir = 0;
        return 0;
    }
}

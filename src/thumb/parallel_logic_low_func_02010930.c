typedef int code();

void func_02010930(int *param_1)
{
    unsigned int flags;
    int *object;

    flags = (unsigned int)param_1[3];
    if (((flags << 2) >> 31) != 0) {
        flags = flags & 0xdfffffff;
        param_1[3] = flags;
        if (((flags << 16) >> 31) == 0) {
            object = (int *)param_1[0x380];
            if (object != 0) {
                (*(code **)(*object + 0xd4))((int)object);
            }
            object = (int *)param_1[0x37f];
            if (object != 0) {
                (*(code **)(*object + 0xd4))((int)object);
            }
        }
    }
}

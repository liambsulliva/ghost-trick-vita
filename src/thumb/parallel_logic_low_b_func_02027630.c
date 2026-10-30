extern unsigned int func_02027c28(void *object);
extern unsigned int func_0205176c(unsigned int value);
extern unsigned int func_02008444(unsigned int value);
extern unsigned int func_02010194(unsigned int value);
extern unsigned int func_02027684(unsigned int value);
extern unsigned int func_0202768c(unsigned int value);
extern unsigned int func_020266d4(unsigned int value);
extern unsigned int func_02027698(unsigned int value);

unsigned int func_02027630(void *object, unsigned int mode)
{
    unsigned int value;
    unsigned int other;

    value = func_0205176c(func_02027c28(object));
    if (value != 0) {
        switch (mode) {
        default:
            other = func_02010194(func_02008444(value));
            break;
        case 0:
            other = func_02027684(func_02008444(value));
            break;
        case 1:
            other = func_0202768c(func_02008444(value));
            break;
        }
        if (other != 0) {
            if (func_020266d4(value) == func_02027698(other)) {
                return 1;
            }
        }
    }
    return 0;
}

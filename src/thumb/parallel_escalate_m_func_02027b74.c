extern int func_020281f0(unsigned char *object, unsigned int id);
extern int func_020265c4(unsigned char *bytes);
extern unsigned short func_020265dc(unsigned char *bytes);

void func_02027b74(unsigned char *command, unsigned char *object)
{
    int left;
    int right;
    int result;

    left = func_020281f0(object, *(unsigned short *)(command + 2));
    right = func_020265c4(command + 4);
    switch (func_020265dc(command + 8)) {
    case 0x1f:
        result = left == right;
        break;
    case 0x20:
        result = left != right;
        break;
    case 0x23:
        result = left > right;
        break;
    case 0x24:
        result = left >= right;
        break;
    case 0x21:
        result = left < right;
        break;
    case 0x22:
        result = left <= right;
        break;
    default:
        result = 0;
        break;
    }
    object[0x10] = result != 0;
}

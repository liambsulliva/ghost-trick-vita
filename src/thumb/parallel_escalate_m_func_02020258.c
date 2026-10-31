struct Field;
extern int func_02020254(struct Field *object);
extern void func_02020310(struct Field *object, int value);
extern void func_0202039c(struct Field *object, unsigned char value);
extern void func_02020408(struct Field *object, unsigned short value);
extern void func_02020474(struct Field *object, unsigned int value);
extern void func_020204e0(struct Field *object, long long value);
extern void func_02020558(struct Field *object, signed char value);
extern void func_020205c4(struct Field *object, short value);
extern void func_02020630(struct Field *object, int value);
extern void func_0202069c(struct Field *object, long long value);
extern void func_02020714(struct Field *object, short value);
extern void func_0202071c(struct Field *object, int value);

void func_02020258(struct Field *object, int value)
{
    switch (func_02020254(object)) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
        func_02020310(object, value != 0);
        break;
    case 4:
        func_0202039c(object, value);
        break;
    case 5:
        func_02020408(object, value);
        break;
    case 6:
        func_02020474(object, value);
        break;
    case 7:
        func_020204e0(object, value);
        break;
    case 8:
        func_02020558(object, value);
        break;
    case 9:
        func_020205c4(object, value);
        break;
    case 10:
        func_02020630(object, value);
        break;
    case 11:
        func_0202069c(object, value);
        break;
    case 12:
        func_02020714(object, value);
        break;
    case 13:
        func_0202071c(object, value);
        break;
    }
}

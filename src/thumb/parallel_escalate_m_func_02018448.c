extern void func_02014bdc(char *object, int enabled);

void func_02018448(char *object, int value)
{
    int enabled;
    switch (value) {
    case 0:
        enabled = 0;
        break;
    default:
        enabled = 1;
        break;
    }
    func_02014bdc(object + 0xf04, enabled);
}

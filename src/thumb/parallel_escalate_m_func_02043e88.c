extern void func_0201c084(int a, unsigned int b, unsigned int c);

void func_02043e88(char *obj)
{
    unsigned char delay;
    int pos;
    int target;

    delay = *(unsigned char *)(obj + 0x13);
    if (delay != 0) {
        *(unsigned char *)(obj + 0x13) = delay - 1;
        return;
    }
    if (*(unsigned char *)(obj + 0x10) == 2) {
        pos = *(int *)(obj + 0x2c);
        target = *(int *)(obj + 0xc8);
        if (target > pos) {
            *(int *)(obj + 0x2c) = pos + *(int *)(obj + 0xcc);
            func_0201c084(*(int *)(obj + 0xd0), 0, -*(int *)(obj + 0xcc));
            if (*(int *)(obj + 0xc8) < *(int *)(obj + 0x2c)) {
                *(int *)(obj + 0x2c) = *(int *)(obj + 0xc8);
                func_0201c084(*(int *)(obj + 0xd0), 0, *(int *)(obj + 0xc8) - *(int *)(obj + 0x2c));
                *(unsigned char *)(obj + 0x10) = 1;
                return;
            }
        } else if (target < pos) {
            *(int *)(obj + 0x2c) = pos - *(int *)(obj + 0xcc);
            func_0201c084(*(int *)(obj + 0xd0), 0, *(int *)(obj + 0xcc));
            if (*(int *)(obj + 0xc8) > *(int *)(obj + 0x2c)) {
                *(int *)(obj + 0x2c) = *(int *)(obj + 0xc8);
                func_0201c084(*(int *)(obj + 0xd0), 0, -(*(int *)(obj + 0xc8) - *(int *)(obj + 0x2c)));
                *(unsigned char *)(obj + 0x10) = 1;
                return;
            }
        } else {
            *(int *)(obj + 0x2c) = target;
            *(unsigned char *)(obj + 0x10) = 1;
        }
    }
}

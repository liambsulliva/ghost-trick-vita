extern void func_0203b190(int *param_1, unsigned int param_2);

void func_0203b1e0(char *object)
{
    int state;
    int value;

    state = *(unsigned char *)(object + 0x10);
    switch (state) {
    case 4:
    case 5:
        value = *(int *)(object + 0xc8);
        if (value == 1) {
            func_0203b190((int *)object, 0x53);
        } else if (value == 2) {
            func_0203b190((int *)object, 0x55);
        }
        *(unsigned char *)(object + 0x10) = 5;
        *(unsigned char *)(object + 0xed) = 10;
        break;
    }
}

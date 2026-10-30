unsigned int func_0202ff58(int param_1);
unsigned int func_0200b7c0(int param_1);

void func_020414e8(char *object)
{
    unsigned int i;
    int value;

    if (func_0202ff58((int)object) != 0) {
        for (i = 0; i < *(unsigned char *)(object + 0xbc); i++) {
            value = *(int *)(object + i * 4 + 0xc0);
            if (value != 0) {
                func_0200b7c0(value);
            }
        }
    }
}

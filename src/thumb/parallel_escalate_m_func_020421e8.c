extern int func_0204224c(unsigned char *object);
extern void func_020427b0(unsigned char *target, int handle, int a, int b, int c, int d, int e, int f, int g, int h, int i);

void func_020421e8(unsigned char *object)
{
    int handle;

    if (object[0] != 0 && object[1] == 0) {
        handle = func_0204224c(object);
        if (object[0x14] != 0) {
            func_020427b0(object + 0x18, handle, 1, 0, 1, 1, *(unsigned short *)(object + 0xc), 1, -1, -1, *(int *)(object + 8));
        } else {
            func_020427b0(object + 0x18, handle, 1, 0, 1, 0, *(unsigned short *)(object + 0xc), 1, -1, -1, *(int *)(object + 8));
        }
        object[1] = 1;
    }
}

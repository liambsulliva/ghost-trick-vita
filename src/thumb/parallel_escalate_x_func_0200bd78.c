typedef struct Slot0bd78 {
    char pad[6];
    short value;
} Slot0bd78;

void func_0200bd78(Slot0bd78 *dest, const int *src)
{
    dest[0].value = src[0] >> 4;
    dest[1].value = src[1] >> 4;
    dest[2].value = src[2] >> 4;
    dest[3].value = src[3] >> 4;
}

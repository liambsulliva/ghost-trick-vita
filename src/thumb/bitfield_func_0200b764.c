typedef struct {
    unsigned char value : 1;
} Bits0200b7640;

int func_0200b764(char *object)
{
    if (((Bits0200b7640 *)(object + 12))->value != 0) {
        return 1;
    }
    return 0;
}

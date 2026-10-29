typedef struct {
    unsigned char reserved : 3;
    unsigned char value : 1;
} Bits0200b7743;

int func_0200b774(char *object)
{
    if (((Bits0200b7743 *)(object + 12))->value != 0) {
        return 1;
    }
    return 0;
}

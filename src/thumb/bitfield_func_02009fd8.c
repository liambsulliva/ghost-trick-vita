typedef struct {
    unsigned char reserved : 2;
    unsigned char value : 1;
} Bits02009fd82;

int func_02009fd8(char *object)
{
    if (((Bits02009fd82 *)(object + 5))->value != 0) {
        return 1;
    }
    return 0;
}

typedef struct {
    unsigned char value : 1;
} Bits02009fc80;

int func_02009fc8(char *object)
{
    if (((Bits02009fc80 *)(object + 5))->value != 0) {
        return 1;
    }
    return 0;
}

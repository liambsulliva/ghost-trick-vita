typedef struct {
    unsigned int value : 1;
} Bits02036f3c0;

int func_02036f3c(char *object)
{
    if (((Bits02036f3c0 *)(object + 20))->value != 0) {
        return 1;
    }
    return 0;
}

extern int func_020100e4(int node);

int func_020100bc(int object)
{
    int node;
    unsigned int bit;
    unsigned int shift;

    shift = 0x39;
    shift = shift << 6;
    node = *(int *)((char *)object + shift);
    bit = *(unsigned int *)(object + 8);
    bit = bit << 0x14;
    bit = bit >> 0x1f;
    if (bit == 1) {
        goto success;
    }
    if (node == 0) {
        goto fail;
    }
    if (func_020100e4(node) == 0) {
        goto fail;
    }
success:
    return 1;
fail:
    return 0;
}

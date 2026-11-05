struct Root {
    unsigned char pad[0x10];
    int bank;
};

extern struct Root data_020b18a0;

unsigned int func_0200286c(int archive, unsigned int *query)
{
    unsigned int offset;
    unsigned int rows;
    unsigned int result;
    unsigned int slot;
    unsigned int row;
    unsigned int n;
    unsigned int word;
    int base;
    unsigned int v;

    result = 1;
    offset = *(unsigned int *)(archive + *(unsigned int *)(int)query * 4 + 0x3c);
    offset = (offset << 8) >> 16;
    rows = *(unsigned int *)(data_020b18a0.bank + 0x1ba4);
    if (rows > 4) {
        rows = 4;
    }
    row = 0xff;
    for (slot = 0; slot < rows; slot++) {
        if (query[0] == *(unsigned char *)(*(unsigned int *)((char *)&data_020b18a0 + 0x10) + slot + 0x1ba8)) {
            row = slot;
            break;
        }
    }
    if (row != 0xff) {
        if (query[1] == 0) {
            result = 0;
        } else {
            base = *(int *)((int)&data_020b18a0 + 0x10) + row * 0x10;
            n = *(unsigned char *)(archive + offset);
            n += query[1];
            n -= 1;
            word = n >> 5;
            base += word * 4;
            v = *(unsigned int *)(base + 0x1540);
            v >>= n - word * 32;
            if ((v & 1) == 0) {
                result = 0;
            }
        }
    }
    return result;
}

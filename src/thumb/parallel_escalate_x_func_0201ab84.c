unsigned char func_0201ab84(void *self, unsigned char *cells, unsigned int index)
{
    unsigned char cell = cells[index >> 1];
    unsigned char value;
    if ((index & 1) == 0) {
        value = cell & 0xf;
    } else {
        value = (cell & 0xf0) >> 4;
    }
    return value;
}

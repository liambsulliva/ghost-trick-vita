extern int data_020cff8c;
extern int data_020cff90;
extern int data_020cff94;
extern int func_02068d08();

int func_02068e7c(unsigned int key)
{
    unsigned int offset = (key << 16) >> 13;
    unsigned int size = ((key & 0x7fff0000) >> 16) << 4;
    unsigned int high = (key & 0x80000000) >> 31;
    int r;

    if (size != 0) {
        if (high != 0) {
            r = func_02068d08(&data_020cff90, &data_020cff94, offset, size);
        } else {
            r = func_02068d08(&data_020cff8c, &data_020cff94, offset, size);
        }
        if (r != 0) {
            return 0;
        }
        return 1;
    }
    return 2;
}

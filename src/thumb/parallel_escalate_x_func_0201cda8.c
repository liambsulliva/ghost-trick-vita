struct Board {
    unsigned char pad[0x10];
    unsigned short width;
    unsigned short height;
};

unsigned int func_0201cda8(struct Board *board, int index)
{
    if (index < 0 || index > board->height - 1) {
        return 1;
    }
    return 0;
}

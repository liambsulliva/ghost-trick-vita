struct Board {
    unsigned char pad[0x10];
    unsigned short width;
    unsigned short height;
};

unsigned int func_0201cdbc(struct Board *board, int index)
{
    if (index < 0 || index > board->width - 1) {
        return 1;
    }
    return 0;
}

struct Counted {
    char pad[0x10];
    unsigned char count;
    char pad11[0x1f];
    unsigned short dirty;
};

void func_02046ed0(struct Counted *p)
{
    if (p->count < 3) {
        p->count = 3;
        p->dirty = 1;
    }
}

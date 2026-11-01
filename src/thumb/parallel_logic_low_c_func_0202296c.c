struct Block0202296c {
    unsigned char pad0[8];
    unsigned short f8;
    unsigned short fa;
    unsigned char padc[2];
    unsigned short fe;
    unsigned char pad10[2];
    unsigned short f12;
    unsigned short f14;
};

extern unsigned char data_020c9d38[];
extern unsigned char data_020c9d50[];

void func_0202296c(void)
{
    struct Block0202296c *b;

    b = (struct Block0202296c *)data_020c9d38;
    if (b->f8 == b->f12) {
        b->f14 = b->f14 - 1;
        if (b->f14 == 0) {
            b->f14 = 4;
            b->fe = b->f8;
            *(unsigned int *)(data_020c9d50 + 0xc) = *(unsigned int *)data_020c9d50;
            return;
        }
        b->fe = 0;
        *(unsigned int *)(data_020c9d50 + 0xc) = 0;
        return;
    }
    b->f14 = 0x1e;
    b->fe = b->fa;
    *(unsigned int *)(data_020c9d50 + 0xc) = *(unsigned int *)(data_020c9d50 + 4);
}

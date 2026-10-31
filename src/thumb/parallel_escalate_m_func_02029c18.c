extern const unsigned char data_02085208[][8];
extern const unsigned char data_02085209[][8];
extern int func_02029ccc(unsigned char *p, int q);
extern int func_02029d04(unsigned int *p);
extern int func_02029d14(unsigned int *p);

void func_02029c18(unsigned char *p, unsigned int *param_2)
{
    unsigned int width;
    unsigned int height;
    unsigned int tile;
    unsigned int x;
    unsigned int y;

    width = data_02085208[(*param_2 & 0xffff) >> 14][(*param_2 >> 30) * 2];
    height = data_02085209[(*param_2 & 0xffff) >> 14][(*param_2 >> 30) * 2];
    tile = (param_2[1] & 0x3ff) << 5;
    for (y = 0; y < height; y += 8) {
        for (x = 0; x < width; x += 8) {
            if (*(unsigned short *)(p + 0x40c) >= 0x80) {
                return;
            }
            if (func_02029ccc(p, *(int *)(p + 4) + tile * 4) != 0) {
                *(int *)(p + *(unsigned short *)(p + 0x40c) * 4 + 0x20c) = *(int *)(p + 4) + tile * 4;
                *(short *)(p + *(unsigned short *)(p + 0x40c) * 4 + 0xc) = x + func_02029d04(param_2);
                *(short *)(p + *(unsigned short *)(p + 0x40c) * 4 + 0xe) = y + func_02029d14(param_2);
                (*(unsigned short *)(p + 0x40c))++;
            }
            tile += 8;
        }
    }
}

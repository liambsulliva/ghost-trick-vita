extern unsigned short data_02084b58[][32];

void func_02018aa4(unsigned char *object)
{
    unsigned char *src;
    unsigned char *dst;
    unsigned char *s;
    unsigned char *d;
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned int m;

    src = *(unsigned char **)(object + 0x68);
    dst = *(unsigned char **)(object + 0x6c);
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 32; j++) {
            d = dst + data_02084b58[i][j];
            for (k = 0; k < 8; k++) {
                s = src + (unsigned short)(j * 8 + (k + i * 8) * 0x100);
                for (m = 0; m < 4; m++) {
                    *d = s[0] | (s[1] << 4);
                    s += 2;
                    d++;
                }
            }
        }
    }
}

void func_0201bb00(unsigned char *object)
{
    unsigned char *src;
    unsigned char *dst;
    unsigned short dst_off;
    unsigned short src_off;
    unsigned int i;
    unsigned int k;
    unsigned int j;
    unsigned int b;

    for (i = 0; i < object[0x76]; i++) {
        for (k = 0; k < *(unsigned short *)(object + 0x74); k++) {
            dst_off = (k << 5) + i * (*(unsigned short *)(object + 0x74) << 5);
            dst = *(unsigned char **)(object + 0x6c) + dst_off;
            for (j = 0; j < 8; j++) {
                src_off = (k + *(unsigned short *)(object + 0x74) * (i * 8 + j)) << 2;
                src = *(unsigned char **)(object + 0x68) + src_off;
                for (b = 0; b < 4; b++) {
                    *dst = *src;
                    dst++;
                    src++;
                }
            }
        }
    }
}

int func_02079180(const char *str1, const char *str2, int len)
{
    const unsigned char *left;
    const unsigned char *right;
    unsigned int align;
    int i;
    unsigned int l1;
    unsigned int r1;

    if (len != 0) {
        left = (const unsigned char *)str1;
        right = (const unsigned char *)str2;
        align = (unsigned int)left & 3;
        i = 0;
        if ((align ^ ((unsigned int)right & 3)) == 0) {
            if (align != 0) {
                for (; i + align < 4 && i < len; i++) {
                    l1 = left[i];
                    r1 = right[i];
                    if (l1 != r1 || l1 == 0) {
                        return l1 - r1;
                    }
                }
            }
            for (; i <= len - 4; i += 4) {
                l1 = *(const unsigned int *)(left + i);
                r1 = *(const unsigned int *)(right + i);
                if (l1 != r1) {
                    break;
                }
                if (~(((l1 & 0x7f7f7f7f) + 0x7f7f7f7f) | l1 | 0x7f7f7f7f)) {
                    break;
                }
            }
        }
        for (; i < len; i++) {
            l1 = left[i];
            r1 = right[i];
            if (l1 != r1 || l1 == 0) {
                return l1 - r1;
            }
        }
    }
    return 0;
}

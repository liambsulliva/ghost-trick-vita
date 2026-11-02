extern unsigned char data_0209be60[][6];
extern void func_0204fbfc(void *obj, int idx, int mode, unsigned char a, unsigned char b);

void func_0204fdc4(void *obj, int group, int mode)
{
    unsigned char i;
    unsigned int j;

    if (group == 0) {
        for (i = 0; i < 12; i++) {
            func_0204fbfc(obj, i, mode, 0, 0);
        }
        return;
    }
    for (j = 0; j < 6; j++) {
        if (data_0209be60[group - 1][j] != 0xff) {
            func_0204fbfc(obj, data_0209be60[group - 1][j], mode, 0, 0);
        }
    }
}

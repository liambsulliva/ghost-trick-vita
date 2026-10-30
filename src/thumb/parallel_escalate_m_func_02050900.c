extern char data_0209c0f0[];
extern char data_0209c0f8[];
extern char data_0209c100[];
extern int func_0200d84c(char *object, int data);
extern signed char *func_02013514(char *object);
extern signed char *func_02012500(char *object);
extern int func_020790c8(const signed char *a, const char *b);
extern int func_02079180(const signed char *a, const char *b, int count);
extern void *func_02008444(void);
extern int func_020509a4(void *system);

int func_02050900(char *self, int data)
{
    int first;
    int second;
    if (func_0200d84c(self + 4, data) != 0) {
        if (*(unsigned char *)(self + 0x5a) == 0) {
            *(unsigned char *)(self + 0x5a) = 1;
            first = func_020790c8(func_02013514(self + 4), data_0209c0f0) == 0;
            second = func_020790c8(func_02013514(self + 4), data_0209c0f8) == 0;
            if (first || second) {
                *(unsigned char *)(self + 0x59) = func_02079180(func_02012500(self + 4), data_0209c100, 4) == 0;
            }
        }
        if (*(unsigned char *)(self + 0x59) != 0 && func_020509a4(func_02008444()) != 0) {
            *(unsigned char *)(self + 0x58) = 0;
        } else {
            *(unsigned char *)(self + 0x58) = 1;
            return 1;
        }
    }
    return 0;
}

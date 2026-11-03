typedef struct Bank0322c {
    char pad[0x13e0];
    unsigned int flags[4];
} Bank0322c;

typedef struct Game0322c {
    char pad[8];
    int f8;
    int fc;
    Bank0322c *bank;
} Game0322c;

extern Game0322c data_020b18a0;
extern unsigned char *func_02003340(unsigned char *p, int arg);

int func_0200322c(unsigned char *p, int arg)
{
    int done;
    int result;
    int n;
    unsigned int *flags;

    done = 0;
    result = 1;
    data_020b18a0.f8 = 0;
    while (!done) {
        if (*p == 0xff) {
            done = 1;
        } else if (*p == 0x81) {
            if (p[1] <= 0x7f) {
                n = p[1];
                flags = &data_020b18a0.bank->flags[0];
                flags += n / 32;
                *flags |= 1 << (n - n / 32 * 32);
            } else {
                result = 0;
                done = 1;
            }
            p += 2;
        } else {
            p = func_02003340(p, arg);
            if (p == 0) {
                result = 0;
                done = 1;
            }
        }
    }
    return result;
}

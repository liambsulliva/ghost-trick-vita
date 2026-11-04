typedef struct Row03298 {
    unsigned int bits[4];
} Row03298;

typedef struct Bank03298 {
    char pad[0x1540];
    Row03298 rows[1];
} Bank03298;

typedef struct Game03298 {
    char pad[8];
    int f8;
    int fc;
    Bank03298 *bank;
} Game03298;

extern Game03298 data_020b18a0;
extern unsigned char *func_02003340(unsigned char *p, int arg);

int func_02003298(unsigned char *p, int arg, int id, int row)
{
    int done;
    int result;
    int match;
    int n;
    Row03298 *rows;
    int off;

    done = 0;
    result = 1;
    match = 0;
    data_020b18a0.f8 = 0;
    while (!done) {
        if (*p == 0xff) {
            done = 1;
        } else if (*p == 0x81) {
            if (p[1] <= 0x7f) {
                if ((unsigned char)id == p[1]) {
                    match = 1;
                } else {
                    match = 0;
                }
            } else {
                result = 0;
                done = 1;
            }
            p += 2;
        } else if (*p <= 0x7f) {
            if (match) {
                n = *p;
                rows = data_020b18a0.bank->rows;
                off = row;
                off <<= 4;
                rows = (Row03298 *)((int)rows + off);
                rows->bits[n / 32] |= 1 << (n - n / 32 * 32);
            }
            p += 2;
            while (*p & 0x80) {
                p++;
            }
            p++;
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

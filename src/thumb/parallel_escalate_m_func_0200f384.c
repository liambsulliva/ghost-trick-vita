extern const signed char data_02083d3c[];
extern const signed char data_02083d3d[];
extern char data_020c9e74;
extern char data_02083d4c;
extern void *func_02008eb0(void);
extern unsigned int func_01ff80b8(void *param_1);
extern int func_0200ee64(int param_1);
extern void func_0206f588(void *param_1, void *param_2, int param_3, int param_4, int param_5, int param_6);

void func_0200f384(char *p)
{
    int i;
    int x;
    int y;
    unsigned short level;

    if (*(unsigned short *)(p + 0xdc6) != 0) {
        *(unsigned short *)(p + 0xdc6) = *(unsigned short *)(p + 0xdc6) - 1;
        if (*(unsigned short *)(p + 0xdc6) != 0) {
            if ((*(unsigned short *)(p + 0xdc6) & 3) == 0) {
                i = (func_01ff80b8(func_02008eb0()) & 7) * 2;
                x = data_02083d3c[i] * 0x1000;
                *(int *)(p + 0xdbc) = x;
                y = data_02083d3d[i] * 0x1000;
                *(int *)(p + 0xdc0) = y;
                level = *(unsigned short *)(p + 0xdc4);
                if (level == 1) {
                    *(int *)(p + 0xdbc) = x * 2;
                    *(int *)(p + 0xdc0) = y * 2;
                } else if (level == 2) {
                    *(int *)(p + 0xdbc) = x * 3;
                    *(int *)(p + 0xdc0) = y * 3;
                } else if (level == 3) {
                    *(int *)(p + 0xdbc) = x * 4;
                    *(int *)(p + 0xdc0) = y * 4;
                }
            }
            *(int *)(p + 0xd9c) += *(int *)(p + 0xdbc);
            *(int *)(p + 0xda0) += *(int *)(p + 0xdc0);
        } else {
            *(int *)(p + 0xdbc) = 0;
            *(int *)(p + 0xdc0) = 0;
        }
        if (((*(unsigned int *)(p + 0xc) << 21) >> 31) == 0 && ((*(unsigned int *)(p + 8) << 16) >> 31) == 0) {
            func_0206f588(&data_020c9e74, &data_02083d4c, 0x80, 0x60, func_0200ee64(*(int *)(p + 0xd9c)), func_0200ee64(*(int *)(p + 0xda0)));
        }
    } else {
        *(int *)(p + 0xdbc) = 0;
        *(int *)(p + 0xdc0) = 0;
    }
}

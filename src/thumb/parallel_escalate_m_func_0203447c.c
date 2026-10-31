struct Flags {
    unsigned int mode : 2;
    unsigned int flag : 1;
};
extern void *func_02008444(void);
extern void *func_0200847c(void);
extern void func_0201135c(void *object);
extern int func_0200f5c8(void *object);
extern int func_0200f5e8(void *object);
extern void func_020330e0(void *object, int value);
extern void func_020330c0(void *object, int value);
extern void func_02034dcc(unsigned char *p);
extern unsigned int func_02010b38(void *object);
extern void func_02032914(unsigned char *p);
extern unsigned int func_02034990(int object);
extern void func_02010ef4(void *object, int a, int b);
extern void func_02010e7c(void *object);
extern void func_02010e9c(void *object);
extern void func_02034b14(unsigned char *p);
extern int func_020349a0(int object);
extern int func_020349a8(int object);
extern void func_02034a6c(unsigned char *p, int a, int b, int c, void *d, void *e, void *f, void *g, void *h, void *i, void *j);
extern void func_02058a80(int *out, int value);
extern void func_02008378(int *out, int *a, int *b);
extern void func_020349b0(int *out, int *in, int scale);
extern void func_020083b0(int *dst, int *src);
extern int func_0202a114(void *object, int kind, int third, int fourth);
extern void func_02034ac4(unsigned char *p);
extern int func_02010fbc(void *object, int value);
extern int func_02010f84(void *object, int value);
extern void func_0206124c(int object);
extern void func_02034aec(unsigned char *p);
extern void func_0202a1b0(void *object, int index, int flag);
extern void func_02011598(void *object);
extern void func_0205e294(int object, int a, int b);
extern void func_02061188(int object);
extern int func_02034a0c(int object);
extern int func_02034a14(int object);
extern void func_02034a44(unsigned char *p, int a, int b, void *table);
extern int func_02034a1c(int object);
extern int func_02034a28(int object);
extern void func_0205e2a4(int object, int other, int *a, int *b);
extern void func_02061198(int object);
extern int func_0205e2b4(int object);
extern void func_020100f4(void *object, int value);
extern void func_020266e8(void *object, int value);
extern void func_020113f8(void *object);
extern unsigned int data_020c9d50[];
extern const char data_0209a2d4[];
extern const char data_0209a2dc[];
extern char data_020a5b2c[];
extern char data_020a5b30[];
extern char data_020a5b34[];
extern char data_020a5b38[];
extern char data_020a5b3c[];
extern char data_020a5b40[];
extern char data_020a5b44[];
extern char data_020a5b48[];
extern char data_020a5b4c[];
extern char data_020a5b50[];
extern char data_020a5b54[];
extern char data_020a5b58[];
extern char data_020a5b5c[];
extern char data_020a5b60[];
extern char data_020a5b64[];
extern char data_020a5b68[];
extern char data_020a5b6c[];
extern char data_020a5b70[];
extern char data_020a5b74[];
extern char data_020a5b78[];
extern char data_020a5b7c[];
extern char data_020a5b80[];
extern char data_020a5b84[];
extern char data_020a5b88[];
extern char data_020a5b8c[];
extern char data_020a5b90[];
extern char data_020a5b94[];
extern char data_020a5b98[];
void func_0203447c(unsigned char *p)
{
    int first[3];
    int second[3];
    int scaled[3];
    int sum[3];
    int end[3];
    int start[3];
    int dx;
    int dy;
    int a;
    int b;
    if (p[6] != 0) {
        p[6]--;
        if (p[6] == 0) {
            func_0201135c(func_02008444());
        }
    }
    if (p[0x3c] != 0) {
        dx = *(int *)(p + 0x40) - func_0200f5c8(func_02008444());
        dy = *(int *)(p + 0x44) - func_0200f5e8(func_02008444());
        if (dx < 0x7c000) {
            func_020330e0(func_02008444(), -0x4000);
        }
        if (dx > 0x84000) {
            func_020330e0(func_02008444(), 0x4000);
        }
        if (dy < 0x5c000) {
            func_020330c0(func_02008444(), -0x4000);
        }
        if (dy > 0x64000) {
            func_020330c0(func_02008444(), 0x4000);
        }
    }
    switch (p[0x3d]) {
    case 0:
        func_02034dcc(p);
        if (func_02010b38(func_02008444()) != 0) {
            if (p[0x3c] == 0) {
                func_02032914(p);
            }
            return;
        }
        if (func_02034990(*(int *)(p + 0x24)) == 0) {
            if (((struct Flags *)(p + 0x38))->mode == 2) {
                func_02010ef4(func_02008444(), 9, 3);
                ((struct Flags *)(p + 0x38))->mode = 0;
            } else if (((struct Flags *)(p + 0x38))->mode == 1) {
                func_02010e7c(func_02008444());
                ((struct Flags *)(p + 0x38))->mode = 0;
            }
            func_02032914(p);
            return;
        }
        if (((struct Flags *)(p + 0x38))->mode == 0) {
            func_02010ef4(func_02008444(), 0xff, -1);
            ((struct Flags *)(p + 0x38))->mode = 1;
        }
        if (p[0x3c] == 0) {
            func_02034b14(p);
            a = func_020349a0(*(int *)(p + 0x24));
            b = func_020349a8(*(int *)(p + 0x24));
            func_02034a6c(p, a, 0x18, 1, data_020a5b44, data_020a5b48, data_020a5b50, data_020a5b70, data_020a5b58, data_020a5b74, data_020a5b34);
            func_02034a6c(p, b, 0x18, 1, data_020a5b38, data_020a5b2c, data_020a5b4c, data_020a5b3c, data_020a5b40, data_020a5b30, data_020a5b98);
            p[0x3c] = 1;
            func_02058a80(first, a);
            func_02058a80(second, b);
            func_02008378(sum, first, second);
            func_020349b0(scaled, sum, 0x800);
            func_020083b0((int *)(p + 0x40), scaled);
            func_0202a114(func_0200847c(), 0x1d, *(int *)(p + 0x40), *(int *)(p + 0x44));
            p[6] = 0x18;
        }
        if (p[0x3c] != 0) {
            p[0x3c]++;
            if (p[0x3c] <= 0x18) {
                return;
            }
            p[0x3c] = 0x18;
            if (((struct Flags *)(p + 0x38))->mode == 1) {
                func_02010e9c(func_02008444());
                ((struct Flags *)(p + 0x38))->mode = 2;
                func_02034ac4(p);
            }
        }
        if ((data_020c9d50[1] & 0xa000) != 0 && func_02010b38(func_02008444()) == 0) {
            if (func_02010fbc(func_02008444(), 9) == 0) {
                return;
            }
            func_0206124c(*(int *)(p + 0x24));
            p[0x3c] = 0;
            func_02034aec(p);
            func_0202a1b0(func_0200847c(), 0x1d, 0x14);
            func_02011598(func_02008444());
            p[6] = 0x20;
            return;
        }
        if (((struct Flags *)(p + 0x38))->flag != 1) {
            if ((data_020c9d50[1] & 0x1000) == 0) {
                return;
            }
            if (func_02010f84(func_02008444(), 8) == 0) {
                return;
            }
            if (func_02010b38(func_02008444()) != 0) {
                return;
            }
        }
        ((struct Flags *)(p + 0x38))->flag = 0;
        func_0202a1b0(func_0200847c(), 0x1d, 0);
        func_0202a114(func_0200847c(), 0x1e, *(int *)(p + 0x40), *(int *)(p + 0x44));
        a = func_020349a0(*(int *)(p + 0x24));
        b = func_020349a8(*(int *)(p + 0x24));
        if (a == 0 || b == 0) {
            return;
        }
        *(short *)(p + 0x3e) = 0;
        p[0x3d] = 1;
        func_02010ef4(func_02008444(), 8, -1);
        func_02011598(func_02008444());
        p[6] = 0x38;
        return;
    case 1:
        (*(short *)(p + 0x3e))++;
        if (*(short *)(p + 0x3e) < 0) {
            return;
        }
        p[0x3d] = 2;
        a = func_020349a0(*(int *)(p + 0x24));
        b = func_020349a8(*(int *)(p + 0x24));
        func_0205e294(*(int *)(p + 0x20), a, b);
        func_02061188(*(int *)(p + 0x24));
        func_02034a44(p, func_02034a0c(*(int *)(p + 0x24)), a, (void *)data_0209a2d4);
        func_02034a44(p, func_02034a14(*(int *)(p + 0x24)), b, (void *)data_0209a2dc);
        return;
    case 2:
        if (func_02034a1c(*(int *)(p + 0x24)) != 0) {
            return;
        }
        if (func_02034a28(*(int *)(p + 0x24)) != 0) {
            return;
        }
        p[0x3d] = 3;
        func_02058a80(end, func_020349a8(*(int *)(p + 0x24)));
        func_02058a80(start, func_020349a0(*(int *)(p + 0x24)));
        func_0205e2a4(*(int *)(p + 0x20), *(int *)(p + 0x24), end, start);
        func_02061198(*(int *)(p + 0x24));
        return;
    case 3:
        if (func_0205e2b4(*(int *)(p + 0x20)) != 0) {
            return;
        }
        p[0x3d] = 4;
        *(short *)(p + 0x3e) = 0;
        a = func_020349a0(*(int *)(p + 0x24));
        b = func_020349a8(*(int *)(p + 0x24));
        func_02034a6c(p, a, 0x20, 0, data_020a5b94, data_020a5b90, data_020a5b8c, data_020a5b88, data_020a5b84, data_020a5b80, data_020a5b7c);
        func_02034a6c(p, b, 0x20, 0, data_020a5b54, data_020a5b5c, data_020a5b78, data_020a5b6c, data_020a5b68, data_020a5b64, data_020a5b60);
        func_020100f4(func_02008444(), a);
        func_020266e8(func_02008444(), b);
        func_02034aec(p);
        return;
    case 4:
        func_020113f8(func_02008444());
        (*(short *)(p + 0x3e))++;
        if (*(short *)(p + 0x3e) > 0x20) {
            p[0x3d] = 0;
            p[0x3c] = 0;
            ((struct Flags *)(p + 0x38))->mode = 1;
        }
        return;
    }
}

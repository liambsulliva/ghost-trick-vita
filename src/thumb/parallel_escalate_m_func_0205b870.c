extern void func_0205bad0(unsigned char *, int);
extern void func_0205bb00(unsigned char *);

void func_0205b870(unsigned char *p)
{
    if (p[0x11] != 0) {
        p[0x11] = p[0x11] - 1;
        return;
    }
    switch (p[0x10]) {
    case 0:
    case 3:
        break;
    case 1:
        func_0205bad0(p, *(int *)(p + 0x90) + *(int *)(p + 0x94));
        if (*(int *)(p + 0x90) > 0x1000) {
            func_0205bad0(p, 0x1000);
            if (p[0x9f] != 0) {
                p[0x9f]--;
                if (p[0xa1] != 0) p[0xa1] = 0;
                p[0x10] = 2;
                return;
            }
            p[0x10] = 3;
            return;
        }
        break;
    case 2:
        func_0205bad0(p, *(int *)(p + 0x90) - *(int *)(p + 0x94));
        if (*(int *)(p + 0x90) < 0x29) {
            func_0205bb00(p);
            if (*(int *)(p + 0x30) != 0) {
                (*(void (**)(int))(*(int *)(*(int *)(p + 0x30)) + 0x28))(*(int *)(p + 0x30));
                (*(void (**)(int))(*(int *)(*(int *)(p + 0x2c)) + 0x28))(*(int *)(p + 0x2c));
            }
            func_0205bad0(p, 0x29);
            if (p[0xa0] != 0) {
                p[0x10] = 4;
                return;
            }
            p[0x10] = 1;
            return;
        }
        break;
    case 4:
        if (*(int *)(p + 0x30) != 0) {
            (*(void (**)(int))(*(int *)(*(int *)(p + 0x30)) + 0x2c))(*(int *)(p + 0x30));
        }
        p[0x10] = p[0x10] + 1;
        break;
    }
}

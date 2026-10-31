extern unsigned int data_020c9d50;
extern int func_02008444(void);
extern int func_0201003c(void);
extern int func_020151b0(int a);
extern void func_02015944(int a);
extern int func_02015aa0(int a);
extern void func_02054a1c(unsigned char *p);
extern void func_02054a2c(unsigned char *p);
extern void func_02054a6c(unsigned char *p);

void func_02054900(unsigned char *p)
{
    switch (p[0x10]) {
    default:
        if (func_020151b0(func_02008444()) != 0 && *(int *)(p + 0x38) == 0) {
            func_02054a1c(p);
        }
        break;
    case 1:
        if (func_020151b0(func_02008444()) != 0) {
            p[0x10] = 3;
            return;
        }
        if (*(int *)(p + 0x38) < 0) {
            if (func_02015aa0(func_0201003c()) != 0 && (data_020c9d50 & 0x200000) != 0) {
                *(int *)(p + 0x38) = *(int *)(p + 0x38) + 0x10;
            } else {
                *(int *)(p + 0x38) = *(int *)(p + 0x38) + 8;
            }
            if (*(int *)(p + 0x38) > 0) {
                *(int *)(p + 0x38) = 0;
            }
        } else {
            *(int *)(p + 0x38) = 0;
            p[0x10] = 2;
            func_02015944(func_0201003c());
        }
        break;
    case 3:
        if (func_02015aa0(func_0201003c()) != 0 && (data_020c9d50 & 0x200000) != 0) {
            *(int *)(p + 0x38) = -0x40;
        }
        if (*(int *)(p + 0x38) > -0x40) {
            *(int *)(p + 0x38) = *(int *)(p + 0x38) - 8;
        } else {
            func_02054a2c(p);
        }
        break;
    }
    func_02054a6c(p);
}

struct Flags {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
};

extern int func_02061448();
extern void func_020082d8();
extern void func_02058a80();
extern void func_020083b0();
extern int func_02008444();
extern void func_020100f4();
extern void func_020608d8();
extern char data_020a46c8;

void func_02060d5c(unsigned char *param_1)
{
    int pos[3];
    int found[3];
    int target[3];
    int other;
    unsigned char i;

    if (*(unsigned short *)(param_1 + 0xec) != 0) {
        return;
    }
    other = func_02061448(param_1);
    func_020082d8(pos, &data_020a46c8);
    if (other != 0) {
        func_02058a80(found, other);
        func_020083b0(pos, found);
    } else if (*(unsigned short *)(param_1 + 0xec) != 0) {
        func_020100f4(func_02008444(), *(int *)(*(int *)(param_1 + 0xf0) + 4));
        func_02058a80(target, *(int *)(*(int *)(param_1 + 0xf0) + 4));
        func_020083b0(pos, target);
    }
    **(int **)(param_1 + 0xd0) = pos[0];
    *(int *)(*(int *)(param_1 + 0xd0) + 4) = pos[1];
    *(int *)(*(int *)(param_1 + 0xd0) + 8) = 0;
    for (i = 1; i < *(unsigned char *)(param_1 + 0x7f); i++) {
        func_020083b0(*(int *)(param_1 + 0xd0) + i * 0xc, *(int *)(param_1 + 0xd0) + (i - 1) * 0xc);
        *(int *)(*(int *)(param_1 + 0xd0) + i * 0xc + 4) += 0x52;
        func_020083b0(*(int *)(param_1 + 0xd4) + i * 0xc, *(int *)(param_1 + 0xd0) + i * 0xc);
    }
    func_020608d8(param_1);
    func_020083b0(param_1 + 0x98, *(int *)(param_1 + 0xd0) + (*(unsigned char *)(param_1 + 0x7f) - 1) * 0xc);
    ((struct Flags *)(param_1 + 0x7c))->b0 = 0;
    ((struct Flags *)(param_1 + 0x7c))->b1 = 1;
    ((struct Flags *)(param_1 + 0x7c))->b3 = 0;
    *(unsigned char *)(param_1 + 0x7e) = 0;
    *(unsigned short *)(param_1 + 0x8c) = 0x400;
    *(unsigned short *)(param_1 + 0x8e) = 0x400;
    *(int *)(param_1 + 0x90) = 0;
    *(int *)(param_1 + 0x94) = 0;
    ((struct Flags *)(param_1 + 0x7c))->b2 = 1;
    *(unsigned char *)(param_1 + 0x80) = 0x1e;
    ((struct Flags *)(param_1 + 0x7c))->b4 = 0;
    *(int *)(param_1 + 0xa8) = 0;
    *(unsigned short *)(param_1 + 0x84) = 0;
    *(unsigned char *)(param_1 + 0x81) = 0;
    *(unsigned char *)(param_1 + 0x82) = 0;
    *(unsigned char *)(param_1 + 0xb0) = 0;
    ((struct Flags *)(param_1 + 0x7c))->b6 = 0;
    ((struct Flags *)(param_1 + 0x7d))->b1 = 1;
    ((struct Flags *)(param_1 + 0x7d))->b2 = 0;
    ((struct Flags *)(param_1 + 0x7d))->b3 = 0;
    *(int *)(param_1 + 0x100) = 0;
    *(int *)(param_1 + 0xfc) = other;
}

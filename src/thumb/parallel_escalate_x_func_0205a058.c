extern int func_020082d8();
extern int func_020083b0();
extern int func_0200f9cc();
extern int func_02027a54();
extern int func_02027a60();
extern int func_02062b60();
extern int func_02062cb8();

void func_0205a058(unsigned char *param_1)
{
    int pos[3];
    unsigned int flags;

    flags = *(unsigned int *)(param_1 + 0x10);
    if ((flags & 1) != 0) {
        if (*(int *)(param_1 + 0x120) >= 0) {
            *(int *)(param_1 + 0x80) = *(int *)(param_1 + 0x80) + *(int *)(param_1 + 0x11c);
            *(int *)(param_1 + 0x120) = *(int *)(param_1 + 0x120) + -1;
        }
        else {
            *(unsigned int *)(param_1 + 0x80) = *(unsigned int *)(param_1 + 0x118);
            *(unsigned int *)(param_1 + 0x10) = flags & ~1;
        }
    }
    if ((*(unsigned int *)(param_1 + 0x10) & 2) != 0) {
        if (*(int *)(param_1 + 0x114) >= 0) {
            func_0200f9cc(param_1 + 0x28, param_1 + 0x108);
            *(int *)(param_1 + 0x114) = *(int *)(param_1 + 0x114) + -1;
        }
        else {
            func_020083b0(param_1 + 0x28, param_1 + 0xfc);
            *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) & ~2;
        }
    }
    flags = *(unsigned int *)(param_1 + 0x10);
    if ((flags & 4) != 0) {
        if (*(int *)(param_1 + 0x12c) >= 0) {
            *(int *)(param_1 + 0x84) = *(int *)(param_1 + 0x84) + *(int *)(param_1 + 0x128);
            *(int *)(param_1 + 0x12c) = *(int *)(param_1 + 0x12c) + -1;
        }
        else {
            *(unsigned int *)(param_1 + 0x84) = *(unsigned int *)(param_1 + 0x124);
            *(unsigned int *)(param_1 + 0x10) = flags & ~4;
        }
    }
    if ((*(unsigned int *)(param_1 + 0x10) & 8) != 0) {
        func_02027a54(param_1, *(unsigned int *)(param_1 + 0x130) & 0xff);
        *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) & ~8;
    }
    if ((*(unsigned int *)(param_1 + 0x10) & 0x10) != 0) {
        func_02027a60(param_1, (int)(short)*(unsigned int *)(param_1 + 0x134));
        *(unsigned int *)(param_1 + 0x10) = *(unsigned int *)(param_1 + 0x10) & ~0x10;
    }
    func_020082d8(pos, param_1 + 0x28);
    func_02062b60(param_1, pos, *(unsigned int *)(param_1 + 0x80), *(unsigned int *)(param_1 + 0x84));
    func_02062cb8(param_1);
    return;
}

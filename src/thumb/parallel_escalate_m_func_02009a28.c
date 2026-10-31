struct Entry {
    unsigned short w;
    unsigned short h;
    unsigned short *list;
};

typedef int code();
extern struct Entry data_02083328[];
extern unsigned int data_020c9e48;
extern void func_02009968();
extern void func_0200a8c8();
extern void func_02009ea0();
extern int func_02008444();
extern void func_02009bdc();
extern void func_02009bbc();
extern void func_02009bcc();
extern void func_02009df4();
extern int func_02007bd8();
extern int *func_02007b70();
extern void func_02009c0c();
extern int func_02009c1c();
extern void func_0202f1bc();
extern void func_02009eec();

void func_02009a28(int param_1, int param_2)
{
    int off;
    unsigned short w;
    unsigned short h;
    int result;
    int flag;
    unsigned short *p;
    unsigned int op;
    int x;
    unsigned short hv;
    int d;
    int buf;
    int *obj;

    func_02009968(param_1);
    off = 0xaa24;
    *(int *)(param_1 + off) = param_2;
    *(unsigned short *)(param_1 + off + 4) = data_02083328[param_2].w;
    *(unsigned short *)(param_1 + off + 6) = data_02083328[param_2].h;
    w = *(unsigned short *)(param_1 + off + 4);
    *(unsigned char *)(param_1 + off + 0x1a) = w >> 4;
    h = *(unsigned short *)(param_1 + off + 6);
    *(unsigned char *)(param_1 + off + 0x1b) = h >> 4;
    func_0200a8c8(param_1, w, h);
    result = 0;
    p = data_02083328[param_2].list;
    flag = 0;
    for (; (op = *p) != 0xffff; p += 4) {
        switch (op) {
        case 0xfffd:
        case 0xfffe:
            if (flag == 0) {
                flag = 1;
                func_02009bbc(func_02008444());
            }
            x = (short)p[1];
            if (x == -1) {
                x = *(unsigned short *)((unsigned char *)param_1 + off + 6);
            }
            hv = x;
            if (*p == 0xfffd) {
                d = *(unsigned short *)((char *)param_1 + off + 6) - (short)p[2];
            } else {
                d = hv - (short)p[2];
            }
            func_02009bcc(func_02008444(), hv, (unsigned short)d);
            break;
        case 0xfffc:
            func_02009bdc(func_02008444(), p[1], p[2]);
            break;
        case 0xfffb:
            func_02009ea0(param_1, (unsigned char)(short)p[1], (unsigned char)((short)p[1] >> 8),
                          (unsigned char)(short)p[2], (unsigned char)((short)p[2] >> 8));
            break;
        case 0xfffa:
            result = 0x7fff;
            break;
        default:
            func_02009df4(param_1, op, (unsigned char)(short)p[1], (unsigned char)(short)p[2], 0);
            break;
        }
    }
    obj = func_02007b70(func_02007bd8());
    buf = (*(code **)(*obj + 0xc))(obj, 0x200);
    if (buf != 0) {
        func_02009c0c(buf, result, 0x200);
        func_0202f1bc(func_02009c1c(), buf, 0x6000, 0x200, 0x10);
    }
    func_02009eec(param_1);
    data_020c9e48 = data_020c9e48 | 0x800;
}

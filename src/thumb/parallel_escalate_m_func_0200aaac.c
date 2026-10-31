typedef int code();

struct Obj0200aaac {
    char pad0[0x10];
    unsigned char mode;
    char pad11[0x17];
    int buffer;
    int count;
    int level;
    int other;
};

extern char data_020c9e48;
extern char data_02083af0;
extern int func_02007b70();
extern int func_02007bd8();
extern int func_02008824();
extern int func_0200883c();
extern int func_02009c1c();
extern int func_0201e888();
extern int func_0202f1bc();
extern int func_0200ab7c();

void func_0200aaac(struct Obj0200aaac *param_1)
{
    unsigned int i;
    unsigned short value;
    int *heap;
    int iVar5;
    int iVar6;
    unsigned short *dst;
    char *base;

    param_1->mode = 0x10;
    i = 0;
    param_1->level = 0xf;
    param_1->count = i;
    param_1->other = i;
    *(unsigned short *)((char *)&data_020c9e48 + 10) = 0x6011;
    iVar6 = func_0201e888(func_02008824(func_0200883c()), 4, 1, 1, 0);
    if (iVar6 != 0) {
        heap = (int *)func_02007b70(func_02007bd8());
        iVar5 = (**(code **)(*heap + 8))(heap, 0x200, (unsigned int)&data_02083af0);
        param_1->buffer = iVar5;
        if (iVar5 != 0) {
            base = (char *)(iVar6 + 4);
            func_0200ab7c(base, iVar5, 0x200);
            func_0202f1bc(func_02009c1c(), base, 0x7e00, 0x20, 0x10);
            func_0202f1bc(func_02009c1c(), base + 0x200, 0, 0x10000, 0xb);
            heap = (int *)func_02007b70(func_02007bd8());
            dst = (unsigned short *)(**(code **)(*heap + 0xc))(heap, 0x800);
            if (dst != 0) {
                func_0202f1bc(func_02009c1c(), dst, 0, 0x800, 0xf);
                value = 0xf000;
                do {
                    *dst++ = value++;
                    i = i + 1;
                } while (i < 0x400);
            }
        }
    }
}

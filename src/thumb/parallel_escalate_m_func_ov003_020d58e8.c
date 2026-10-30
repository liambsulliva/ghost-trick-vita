typedef int code();
extern int func_0200883c();
extern int func_0201a4e8();
extern int func_02007bd8();
extern int func_02007b70();
extern int func_0201e888();
extern int func_02009c1c();
extern int func_0202f1bc();
extern unsigned char data_020c9e88[];

void func_ov003_020d58e8(int param_1)
{
    int iVar1;
    int iVar2;
    int iVar5;
    int base;
    int objbuf;
    int table;
    int row;
    int src;
    int dst;
    int big;
    int one;
    unsigned int quot;
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned int l;
    unsigned int y;
    unsigned int x;

    i = 0;
    *(unsigned short *)(data_020c9e88 + 0x2e) = i;
    func_0200883c();
    iVar1 = func_0201a4e8();
    func_02007bd8();
    iVar2 = func_02007b70();
    quot = *(int *)(param_1 + 0x28) % 18;
    one = 1;
    iVar5 = func_0201e888(iVar1, 0, (unsigned short)(quot * 5 + 0x417), one, iVar2);
    base = iVar5 + *(int *)(iVar5 + 0xc);
    big = 0xc000;
    func_0202f1bc(func_02009c1c(), iVar5 + *(int *)(iVar5 + 0x14), 0, one << 9, 0x15);
    func_02007bd8();
    iVar2 = func_02007b70();
    objbuf = (**(code **)(*(int *)iVar2 + 0xc))(iVar2, big);
    for (; i < 32; i++) {
        row = base + i * 8;
        for (j = 0; j < 24; j++) {
            src = row + j * 0x800;
            dst = objbuf + ((j * 32 + i) * 64);
            for (k = 0; k < 8; k++) {
                for (l = 0; l < 8; l++) {
                    *(unsigned char *)(dst + k + l * 8) = *(unsigned char *)(src + k + l * 0x100);
                }
            }
        }
    }
    func_0202f1bc(func_02009c1c(), objbuf, 0, big, 0x19);
    func_02007bd8();
    iVar2 = func_02007b70();
    table = (**(code **)(*(int *)iVar2 + 0xc))(iVar2, big >> 5);
    for (x = 0; x < 32; x++) {
        for (y = 0; y < 24; y++) {
            ((unsigned short *)table)[x + y * 32] = x + y * 32;
        }
    }
    func_0202f1bc(func_02009c1c(), table, 0, 0x600, 0x1d);
}

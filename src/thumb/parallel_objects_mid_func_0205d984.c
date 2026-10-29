extern unsigned char data_020cff60;
extern int func_0202c358();
extern int func_02056cb8();

void func_0205d984(int param_1)

{
    int iVar1;
    int iVar2;
    unsigned char b;
    unsigned char *p;

    func_02056cb8(param_1);
    iVar1 = func_0202c358(param_1);
    if (iVar1 != 0) {
        b = *(unsigned char *)(param_1 + 0x7e);
        if (b >= 0x1a) {
            p = (unsigned char *)param_1;
            p = p + 0x7e;
            *p = 0x1a;
            return;
        }
        {
            char *w;
            w = (char *)param_1;
            w = w + 0x90;
            iVar1 = *(int *)w;
        }
        iVar1 = iVar1 + (((volatile unsigned char *)&data_020cff60)[1] + 1);
        *(int *)((char *)param_1 + 0x90) = iVar1;
        if (iVar1 > 0x20) {
            *(int *)((char *)param_1 + 0x90) = 0x20;
        }
        iVar2 = *(unsigned char *)(param_1 + 0x7e);
        iVar1 = *(int *)(param_1 + 0x90) + iVar2;
        if (iVar2 > 0x3e) {
            iVar2 = 0x3e;
        }
        if (iVar1 > 0x3f) {
            iVar1 = 0x3f;
        }
        *(int *)(param_1 + 0x80) = iVar2 << 0xc;
        *(int *)(param_1 + 0x84) = iVar1 << 0xc;
        iVar1 = *(unsigned char *)(param_1 + 0x7e);
        p = (unsigned char *)param_1;
        p = p + 0x7e;
        *p = iVar1 + (unsigned char)((((volatile unsigned char *)&data_020cff60)[1] + 1) * 2);
    }
    return;
}

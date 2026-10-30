extern unsigned int * func_02008444();
extern int func_02010194();
extern void func_02058a80();

int func_02012318(int param_1)

{
    int iVar1;
    int iVar2;
    int uVar3;
    unsigned int i;
    int local[3];
    int lim;

    iVar1 = func_02010194((int)func_02008444());
    if (iVar1 != 0) {
        func_02058a80(local,iVar1);
        lim = local[1] >> 12;
        for (i = 0; i < 4u; i = i + 1) {
            iVar2 = param_1 + i * 2;
            uVar3 = *(unsigned short *)(iVar2 + 6);
            if (uVar3 > lim && (int)*(unsigned short *)(iVar2 + 0xe) < lim) {
                uVar3 = uVar3 - 0xc0;
                if (uVar3 < 0) {
                    uVar3 = 0;
                }
                return uVar3 << 12;
            }
        }
        lim = lim - 0x60;
        if (lim < 0) {
            lim = 0;
        }
        return lim << 12;
    }
    return -1;
}

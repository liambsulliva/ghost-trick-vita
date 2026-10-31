extern char data_020b18a0;
extern int func_02002be0();
extern int func_02002c24();
extern void func_02003a70();
extern void func_02007948();

int func_02002c74(unsigned int param_1, unsigned int param_2)
{
    int iVar4;
    unsigned short *puVar5;
    int bVar1;

    bVar1 = 0;
    puVar5 = (unsigned short *)(*(char **)((char *)&data_020b18a0 + 0x10) + 0x1a7c + param_1 * 8);
    if (*((unsigned char *)puVar5 + 2) == *((unsigned char *)puVar5 + 3)) {
        iVar4 = func_02002be0(param_1 & 0xff);
        if (*(unsigned char *)(iVar4 + 0x10) <= param_2) {
            func_02003a70(*(unsigned char *)(iVar4 + 0x30));
            bVar1 = 1;
        } else {
            return bVar1;
        }
    } else if (*(unsigned char *)(*(char **)((char *)&data_020b18a0 + 0x10) + 0x1b7c) < 0x10) {
        iVar4 = func_02002c24();
        if (iVar4 == 0) {
            iVar4 = func_02002be0(0xff);
            if (*(unsigned char *)(iVar4 + 0x10) <= param_2) {
                func_02003a70(*(unsigned char *)(iVar4 + 0x30));
                bVar1 = 1;
            } else {
                return bVar1;
            }
        }
    } else {
        iVar4 = func_02002be0(0xff);
        if (*(unsigned char *)(iVar4 + 0x10) <= param_2) {
            func_02003a70(*(unsigned char *)(iVar4 + 0x30));
            bVar1 = 1;
        } else {
            return bVar1;
        }
    }
    if (bVar1 != 0) {
        func_02007948();
    }
    *puVar5 = *puVar5 | (unsigned short)(1 << *(unsigned char *)(iVar4 + 0x30));
    *((unsigned char *)puVar5 + 3) = *((unsigned char *)puVar5 + 3) + 1;
    *(signed short *)(*(char **)((char *)&data_020b18a0 + 0x10) + 0x1b7e) = *(signed short *)(*(char **)((char *)&data_020b18a0 + 0x10) + 0x1b7e) | (signed short)(1 << *(unsigned char *)(iVar4 + 0x30));
    *(unsigned char *)(*(char **)((char *)&data_020b18a0 + 0x10) + 0x1b7c) = *(unsigned char *)(*(char **)((char *)&data_020b18a0 + 0x10) + 0x1b7c) + 1;
    return iVar4;
}

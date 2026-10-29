extern char data_020c9e88;
void func_0206576c(int param_1,unsigned short param_2)

{
    char cVar1;
    unsigned short uVar2;

    cVar1 = *(char *)(param_1 + 0x14);
    uVar2 = (unsigned short)((unsigned int)0x000001ffu);
    if (cVar1 == '\x03') {
        *(unsigned short *)(((int)&data_020c9e88) + 0x2e) = param_2 & uVar2;
        return;
    }
    if (cVar1 == '\x02') {
        *(unsigned short *)(((int)&data_020c9e88) + 0x2a) = param_2 & uVar2;
        return;
    }
    if (cVar1 == '\x01') {
        *(unsigned short *)(((int)&data_020c9e88) + 0x26) = param_2 & uVar2;
        return;
    }
    if (cVar1 == '\0') {
        *(unsigned short *)(((int)&data_020c9e88) + 0x22) = param_2 & uVar2;
    }
    return;
}

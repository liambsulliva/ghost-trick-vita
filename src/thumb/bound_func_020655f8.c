extern char data_020c9e48;
void func_020655f8(int param_1,unsigned short param_2)

{
    char cVar1;
    unsigned short uVar2;

    cVar1 = *(char *)(param_1 + 0x14);
    uVar2 = (unsigned short)((unsigned int)0x000001ffu);
    if (cVar1 == '\x03') {
        *(unsigned short *)(((int)&data_020c9e48) + 0x1a) = param_2 & uVar2;
        return;
    }
    if (cVar1 == '\x02') {
        *(unsigned short *)(((int)&data_020c9e48) + 0x16) = param_2 & uVar2;
        return;
    }
    if (cVar1 == '\x01') {
        *(unsigned short *)(((int)&data_020c9e48) + 0x12) = param_2 & uVar2;
        return;
    }
    if (cVar1 == '\0') {
        *(unsigned short *)(((int)&data_020c9e48) + 0xe) = param_2 & uVar2;
    }
    return;
}

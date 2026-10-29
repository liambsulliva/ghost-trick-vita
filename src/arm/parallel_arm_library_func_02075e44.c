struct field_a {
    unsigned short low3 : 3;
};
struct field_b {
    unsigned char low4 : 4;
};
void func_02072ea8(int, int, unsigned int);

void func_02075e44(unsigned char *param_1)
{
    unsigned char *iVar1;
    unsigned char *iVar2;

    iVar1 = (unsigned char *)0x02fffc80;
    iVar2 = iVar1 + 6;
    param_1[0] = ((struct field_a *)(iVar1 + 100))->low3;
    param_1[1] = ((struct field_b *)(iVar1 + 2))->low4;
    param_1[2] = iVar1[3];
    param_1[3] = iVar1[4];
    *(unsigned short *)(param_1 + 0x1a) = iVar1[0x1a];
    *(unsigned short *)(param_1 + 0x52) = iVar1[0x50];
    func_02072ea8((int)iVar2, (int)(param_1 + 4), 20);
    func_02072ea8((int)(iVar1 + 0x1c), (int)(param_1 + 0x1c), 52);
    *(unsigned short *)(param_1 + 0x18) = 0;
    *(unsigned short *)(param_1 + 0x50) = 0;
}

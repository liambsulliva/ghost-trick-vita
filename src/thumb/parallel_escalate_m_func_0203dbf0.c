typedef int code();
extern int func_02064478();
extern int func_02064464();
void func_0203dbf0(int *param_1)
{
    unsigned int uVar1;
    unsigned int *puVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    unsigned char *puVar5;

    puVar2 = (unsigned int *)func_02008444();
    func_0201192c((int)puVar2, 0x17);
    uVar1 = func_02071e40();
    *(unsigned int *)(param_1 + 0x10) = (unsigned int)uVar1;
    puVar2 = (unsigned int *)func_02008444();
    uVar3 = func_0203dcb4((char *)puVar2);
    *(short *)(param_1 + 0x8) = (short)uVar3;
    puVar2 = (unsigned int *)func_02008444();
    uVar4 = func_0200f178((int)puVar2);
    *(unsigned int *)(param_1 + 0x9) = uVar4;
    puVar2 = (unsigned int *)func_02008444();
    uVar4 = func_0200f190((int)puVar2);
    *(unsigned int *)(param_1 + 0xa) = uVar4;
    puVar2 = (unsigned int *)func_02008444();
    puVar2 = (unsigned int *)func_0201230c((char *)puVar2);
    func_020083b0((int *)(param_1 + 0xb), (int *)puVar2);
    puVar2 = (unsigned int *)func_02008444();
    func_02011a14((int)puVar2, 0);
    puVar2 = (unsigned int *)func_02008444();
    func_02011a28((int)puVar2, 0);
    puVar2 = (unsigned int *)func_02008444();
    func_020267ac((char *)puVar2, 0);
    puVar2 = (unsigned int *)func_02008444();
    func_020267b8((char *)puVar2, 0);
    puVar2 = (unsigned int *)func_02008444();
    uVar4 = 0;
    func_02009bdc((char *)puVar2, 0, 0);
    puVar2 = (unsigned int *)func_02008444();
    func_0200ec24((int)puVar2, 1);
    param_1[0xe] = 0;
    if (param_1[2] == 9) {
        param_1[0xe] = func_02064478(0x020cfdb4);
    }
    else {
        param_1[0xe] = func_02064464(0x020cfd98);
    }
    if (param_1[0xe] != 0) {
        func_0203dcbc((char *)param_1[0xe], param_1[2]);
        puVar5 = (unsigned char *)func_02009900();
        func_0202f7e8((int)puVar5, param_1[0xe], 1, 0);
    }
    param_1[0xf] = 0;
}

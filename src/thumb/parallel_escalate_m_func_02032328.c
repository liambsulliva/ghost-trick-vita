extern char data_02099dd4;
extern char data_02099ddc;
extern char data_02099de8;
extern char data_02099dfc;
extern char data_02099e00;
extern char data_02099e08;
extern char data_02099e14;
extern char data_02099e2c;
extern char data_02099e44;
extern char data_02099e58;
extern char data_020ce7d4;
extern unsigned int *func_02008444();
extern unsigned int func_020283b0();
extern void func_02010384();
extern void func_02011454();
extern unsigned int func_02011320();
extern void func_0201192c();
extern int func_02031b20();
extern void func_02022f34();
extern void func_02010d18();
extern void func_0200da04();
extern int func_02027518();
extern void func_02027538();
extern unsigned int *func_02009900();
extern unsigned int func_0202fa8c();
extern unsigned char func_0203247c();
extern int func_02012518();

void func_02032328(unsigned char *param_1)
{
    unsigned int uVar3;
    unsigned int uVar5;
    unsigned int uVar8;
    unsigned int pcVar1;
    unsigned int pcVar7;
    unsigned int *puVar2;
    unsigned int *puVar6;
    int iVar4;
    int iVar6;

    param_1[0x20] = 0;
    param_1[0x21] = 0;
    param_1[0x22] = 0;
    puVar2 = func_02008444();
    uVar3 = func_020283b0(puVar2);
    param_1[0x23] = (unsigned char)uVar3;
    puVar2 = func_02008444();
    func_02010384(puVar2, 0);
    puVar2 = func_02008444();
    func_02011454(puVar2);
    param_1[4] = 0;
    puVar2 = func_02008444();
    uVar3 = func_02011320(puVar2);
    switch (uVar3) {
    case 1:
        puVar2 = func_02008444();
        func_0201192c(puVar2, 2);
        iVar6 = func_02031b20(param_1);
        func_02022f34(iVar6, ((unsigned int)&data_02099de8), 0);
        pcVar1 = ((unsigned int)&data_02099dfc);
        uVar5 = ((unsigned int)&data_02099dd4);
        uVar8 = ((unsigned int)&data_02099e00);
        func_0200da04(pcVar1, uVar5, ((unsigned int)&data_02099ddc), uVar8);
        iVar4 = 0x1004;
        if (func_02027518(iVar4) != 0) {
            func_02027538(iVar4);
            pcVar7 = ((unsigned int)&data_02099e08);
            func_0200da04(pcVar1, uVar5, pcVar7, uVar8);
        }
        else if (func_02027518(iVar4 + 1) != 0) {
            func_02027538(iVar4 + 1);
            pcVar7 = ((unsigned int)&data_02099e14);
            func_0200da04(pcVar1, uVar5, pcVar7, uVar8);
        }
        puVar6 = func_02009900();
        uVar3 = func_0202fa8c(puVar6, ((unsigned int)&data_020ce7d4));
        if (uVar3 != 0) {
            func_0200da04(&data_02099dfc, &data_02099dd4, &data_02099e2c, &data_02099e00);
        }
        break;
    case 3:
        iVar6 = func_02031b20(param_1);
        func_02022f34(iVar6, ((unsigned int)&data_02099e44), 1);
        puVar2 = func_02008444();
        func_02010d18(puVar2);
        param_1[4] = 8;
        break;
    case 2:
        iVar6 = func_02031b20(param_1);
        func_02022f34(iVar6, ((unsigned int)&data_02099de8), 0);
        break;
    }
    puVar2 = func_02008444();
    uVar3 = func_0203247c(puVar2);
    if (uVar3 == 4) {
        puVar2 = func_02008444();
        uVar5 = ((unsigned int)&data_02099dd4);
        iVar4 = ((unsigned int)&data_02099e58);
        iVar6 = func_02012518(puVar2, uVar5, iVar4);
        if (iVar6 != 0) {
            func_0200da04(((unsigned int)&data_02099dfc), uVar5, iVar4, ((unsigned int)&data_02099e00));
        }
    }
}

struct Ov1Obj {
    unsigned char pad0[5];
    unsigned char state;
    unsigned char pad6[26];
    int field20;
};
extern unsigned char data_020c9e88[];
extern unsigned char data_020c9e48[];
extern unsigned char data_020cff60[];
extern unsigned char data_020c9d50[];
extern unsigned char data_ov001_020d3e10[];
extern unsigned char data_ov001_020d3d58[];
extern unsigned char data_ov001_020d3e04[];
extern unsigned char data_ov001_020d3e1c[];
extern unsigned char data_ov001_020d3d8c[];
extern unsigned char data_ov001_020d3e28[];
extern int func_02010b7c();
extern int func_02015154();
extern int func_0201572c();
extern int func_02022f34();
extern int func_02023bd4();
extern int func_020303c0();
extern int func_02031b20();
extern int func_020521d4();
extern int func_ov001_020d3d04();

void func_ov001_020d3b60(struct Ov1Obj *param_1)
{
    int iVar2;
    unsigned int uVar3;
    unsigned int uVar4;

    switch (param_1->state) {
    case 0:
        func_02023bd4(data_ov001_020d3e10);
        *(unsigned int *)(data_020c9e48 + 0x54) = 0x00210810;
        func_ov001_020d3d04(param_1, data_ov001_020d3d58);
        *(volatile unsigned short *)(data_020c9e88 + 0x2c) = 0;
        *(volatile unsigned short *)(data_020c9e88 + 0x2e) = 0;
        *(volatile unsigned short *)(data_020c9e48 + 0x18) = 0;
        *(volatile unsigned short *)(data_020c9e48 + 0x1a) = 0;
        *(unsigned int *)(data_020c9e48) = 0xfffff7ff & *(unsigned int *)(data_020c9e48);
        func_020521d4(data_ov001_020d3e04, 0, 0xf, 0x100);
        func_020521d4(data_ov001_020d3e04, 1, 0xf, 0x100);
        param_1->field20 = 0x5a;
        param_1->state = param_1->state + 1;
        return;
    case 1:
    case 3:
        iVar2 = func_0201572c();
        if (iVar2 == 0) {
            if (data_020cff60[0x10] == 1) {
                func_02010b7c();
                iVar2 = func_02015154();
                if ((iVar2 != 0) || ((*(unsigned int *)(data_020c9d50 + 4) & 0x41) != 0)) {
                    param_1->field20 = 0;
                }
            }
            if (param_1->field20 != 0) {
                param_1->field20 = param_1->field20 + -1;
            }
            if (param_1->field20 == 0) {
                func_020521d4(data_ov001_020d3e1c, 0, 0xf, 0x100);
                func_020521d4(data_ov001_020d3e1c, 1, 0xf, 0x100);
                param_1->state = param_1->state + 1;
                return;
            }
        }
        break;
    case 2:
        iVar2 = func_0201572c();
        if (iVar2 == 0) {
            func_ov001_020d3d04(param_1, data_ov001_020d3d8c);
            *(unsigned short *)(data_020c9e48 + 0xa) = 0xff7f & *(unsigned short *)(data_020c9e48 + 0xa);
            *(unsigned int *)(data_020c9e48) = (*(unsigned int *)(data_020c9e48) & 0xffffeaff) | 0x800;
            func_020521d4(data_ov001_020d3e04, 0, 0xf, 0x100);
            func_020521d4(data_ov001_020d3e04, 1, 0xf, 0x100);
            param_1->field20 = 0x5a;
            param_1->state = param_1->state + 1;
            return;
        }
        break;
    default:
        iVar2 = func_0201572c();
        if (iVar2 == 0) {
            uVar3 = *(unsigned int *)(data_020c9e48 + 0x54) & 0xfffff7ff;
            uVar4 = *(unsigned int *)(data_020c9e48) & 0xfffff7ff;
            *(unsigned int *)(data_020c9e48 + 0x54) = uVar3 | 0x1000;
            *(unsigned int *)(data_020c9e48) = uVar4 | 0x1100;
            func_020303c0();
            uVar3 = func_02031b20(param_1);
            func_02022f34(uVar3, data_ov001_020d3e28, 0);
        }
    }
    return;
}

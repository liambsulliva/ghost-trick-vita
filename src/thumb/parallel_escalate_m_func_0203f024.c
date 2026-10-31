typedef int code();
extern int func_0203f4f8();
extern int func_0203f1f4();
extern int func_02071dd0();
extern int func_02071960();
extern int func_0203f4ac();
extern int func_0202f64c();

void func_0203f024(char *param_1)
{
    unsigned char uVar1;

    if (*(unsigned char *)(param_1 + 6) != 0) {
        *(unsigned char *)(param_1 + 6) = *(unsigned char *)(param_1 + 6) - 1;
        return;
    }
    switch (*(unsigned char *)(param_1 + 4)) {
    case 0:
    case 2:
    case 5:
    default:
        goto code_r0x0203f098;
    case 1:
        if (*(int *)(param_1 + 0x3c) != 0) {
            *(int *)(param_1 + 0x3c) = *(int *)(param_1 + 0x3c) - 1;
            *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x24) + *(int *)(param_1 + 0x34);
            *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x28) + *(int *)(param_1 + 0x38);
        }
        if (*(int *)(param_1 + 0x3c) != 0) goto code_r0x0203f098;
        *(int *)(param_1 + 0x24) = *(int *)(param_1 + 0x2c);
        *(int *)(param_1 + 0x28) = *(int *)(param_1 + 0x30);
        uVar1 = 2;
        break;
    case 3:
        func_0203f4f8(param_1);
        func_0203f1f4(param_1);
        uVar1 = 4;
        break;
    case 4:
        func_02071dd0();
        func_02071960(0x80);
        *(int *)(param_1 + 0x20) = 0;
        uVar1 = 5;
        break;
    }
    *(unsigned char *)(param_1 + 4) = uVar1;
code_r0x0203f098:
    func_0203f4ac(param_1);
    if (*(unsigned char *)(param_1 + 5) != 0) {
        *(unsigned char *)(param_1 + 5) = *(unsigned char *)(param_1 + 5) - 1;
        if (*(unsigned char *)(param_1 + 5) == 0) {
            func_0202f64c(param_1 + 0x4c);
        }
    }
}

typedef int code();
extern void func_0202f64c(char *object);
extern unsigned char func_ov006_020d41c8(char *object, unsigned char a, unsigned char b, unsigned int c);

void func_ov006_020d4098(char *param_1, unsigned char param_2, unsigned char param_3, unsigned char param_4, unsigned int param_5)
{
    int *piVar2;
    unsigned int uVar3;
    char *iVar4;
    unsigned char uVar5;

    *(unsigned char *)(param_1 + 0x24) = param_2;
    *(unsigned char *)(param_1 + 0x25) = param_3;
    *(unsigned char *)(param_1 + 0x26) = param_4;
    *(unsigned int *)(param_1 + 0x2c) = param_5;
    *(unsigned char *)(param_1 + 0x25) = func_ov006_020d41c8(param_1, *(const unsigned char *)(param_1 + 0x24), *(const unsigned char *)(param_1 + 0x25), param_5);
    if (*(unsigned char *)(param_1 + 0x26) == 0) {
        func_0202f64c(param_1 + 0x64);
    }
    uVar3 = 0;
    do {
        iVar4 = param_1 + uVar3 * 4;
        piVar2 = *(int **)(iVar4 + 0x68);
        if (piVar2 != 0) {
            (**(code **)(*piVar2 + 0x20))(piVar2, param_1);
            *(unsigned int *)(iVar4 + 0x68) = 0;
        }
        uVar3 = uVar3 + 1;
    } while (uVar3 < 2);
    if (*(unsigned char *)(param_1 + 0x24) != 0) {
        func_0202f64c(param_1 + 0x4c);
        func_0202f64c(param_1 + 0x58);
        func_0202f64c(param_1 + 0x5c);
        func_0202f64c(param_1 + 0x60);
        uVar5 = 0;
        do {
            func_0202f64c(param_1 + 0x70 + uVar5 * 4);
            uVar5 = uVar5 + 1;
        } while (uVar5 < 9);
    } else {
        func_0202f64c(param_1 + 0x54);
    }
    *(unsigned char *)(param_1 + 0x27) = 2;
}

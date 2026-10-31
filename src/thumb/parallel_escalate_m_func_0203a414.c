typedef int code();
extern int func_0202f624();
extern int func_0203a540();
extern int func_0205765c();
extern int func_02009900();
extern int func_0202f7e8();

void func_0203a414(int *param_1)
{
    int *piVar2;
    unsigned char *puVar3;

    if (*param_1 == 0) {
        piVar2 = (int *)func_0202f624(0xfc);
        if (piVar2 != (int *)0x0) {
            piVar2 = (int *)func_0203a540(piVar2, param_1[1]);
        }
        *param_1 = (int)piVar2;
        if (piVar2 != (int *)0x0) {
            (**(code **)(*piVar2 + 0xa8))(piVar2, 0x56, 0, 1, 0, 1, -1, 0);
            (**(code **)(*(int *)*param_1 + 0x84))((int *)*param_1, 0x420);
            func_0205765c(*param_1, 0, 0, 0x100000, 0x100000);
            (**(code **)(*(int *)*param_1 + 0xb8))((int *)*param_1, 0x100000, 0x100000);
            (**(code **)(*(int *)*param_1 + 0x34))((int *)*param_1, 0x84000, 0xc000, 0);
            puVar3 = (unsigned char *)func_02009900();
            func_0202f7e8(puVar3, *param_1, 1, 0);
        }
    }
}

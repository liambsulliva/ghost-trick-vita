extern int func_02033d8c();
extern int func_02009900();
extern void func_0202f7e8();
extern void func_ov006_020d4260();

void func_ov006_020d420c(unsigned char *param_1)
{
    unsigned int value;

    if (param_1[0x28] == 1) {
        return;
    }
    if (param_1[0x28] == 2) {
        return;
    }
    param_1[0x28] = 1;
    if (*(unsigned int *)(param_1 + 0x94) == 0) {
        value = func_02033d8c(0x020cd8b0u);
        *(unsigned int *)(param_1 + 0x94) = value;
        value = func_02009900(value);
        func_0202f7e8(value, *(unsigned int *)(param_1 + 0x94), 1, 0);
    }
    if (*(unsigned int *)(param_1 + 0x94) != 0) {
        func_ov006_020d4260(*(unsigned int *)(param_1 + 0x94), 8);
    }
}

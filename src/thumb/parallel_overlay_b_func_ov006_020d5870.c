unsigned int func_ov006_020d42b8(int);
void func_020521d4(unsigned int, int, int, int);

void func_ov006_020d5870(unsigned char *param_1, int param_2)
{
    int value;

    if (func_ov006_020d42b8(param_2) != 0) {
        param_1[0x24] = 9;
        value = 6;
        func_020521d4(0x020d8df4u, 0, value, value + 0xfa);
        func_020521d4(0x020d8df4u, 1, value, value + 0xfa);
    }
}

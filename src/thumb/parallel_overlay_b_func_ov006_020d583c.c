void func_020521d4(unsigned int, int, int, int);
void func_ov006_020d6c20();

void func_ov006_020d583c(unsigned char *param_1)
{
    int value;

    func_ov006_020d6c20(param_1);
    param_1[0x24] = 7;
    value = 6;
    func_020521d4(0x020d8de8u, 0, value, value + 0xfa);
    func_020521d4(0x020d8de8u, 1, value, value + 0xfa);
}

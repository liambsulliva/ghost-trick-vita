unsigned int func_0203ad9c(int param_1, unsigned int param_2)
{
    if (param_2 >= 3) {
        if (param_2 == 6) {
            return 0x4000;
        }
        if (param_2 == 9) {
            return 0x8000;
        }
        return ((param_2 - 3) * 0x1555) & 0xffff;
    }
    return (0x1555 * param_2 + 0xc000) & 0xffff;
}

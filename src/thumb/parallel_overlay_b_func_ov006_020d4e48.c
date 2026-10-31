void func_ov006_020d4e48(unsigned char *param_1, int param_2)
{
    unsigned int *word = (unsigned int *)(param_1 + 0xc);
    unsigned int old = *word;
    *word = (0xf7ffffffu & old) | ((param_2 & 1) << 27);
}

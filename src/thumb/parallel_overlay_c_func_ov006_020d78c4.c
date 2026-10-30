extern int func_ov006_020d7594(unsigned char *, unsigned int, int, int);
extern void func_02014ddc(void *, int);

void func_ov006_020d78c4(unsigned char *param_1, unsigned int param_2, int param_3)
{
    signed char i;
    int *a;
    int *b;
    signed char *c;
    int v;

    a = (int *)(param_1 + 0x438 + param_2 * 0x84);
    b = (int *)(param_1 + 0x540 + param_2 * 0x84);
    c = (signed char *)(param_1 + param_2);
    for (i = 0; i < 0x21; i++) {
        if (a[i] != 0) {
            v = func_ov006_020d7594(param_1, param_2, c[0x32], i) << 12;
            func_02014ddc((void *)a[i], v + (param_3 << 12));
            if (b[i] != 0) {
                func_02014ddc((void *)b[i], v + (param_3 << 12));
            }
        }
    }
}

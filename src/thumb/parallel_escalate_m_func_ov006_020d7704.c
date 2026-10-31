extern void func_0202f64c(int *);
extern int func_ov006_020d7544(unsigned char *, unsigned int, int);
extern int func_ov006_020d7594(unsigned char *, unsigned int, int, int);
extern void func_02014ddc(int, int);
extern void func_ov006_020d759c(unsigned char *, int, int, int, int);
extern void func_ov006_020d7c8c(int, int, int, int, signed char);

void func_ov006_020d7704(unsigned char *param_1, int param_2, int param_3, unsigned char param_4)
{
    unsigned int bit;
    int *a;
    int v1;
    int v2;
    signed char i;
    int *b;

    bit = ((unsigned int)param_1[0x25] << 23) >> 24;
    a = (int *)(param_1 + 0x438 + param_3 * 0x84);
    b = (int *)(param_1 + 0x540 + param_3 * 0x84);
    if (param_3 != 0 || bit != 0) {
        for (i = 0; i < 0x21; i++) {
            func_0202f64c(&b[i]);
        }
    }
    if (bit != 0) {
        return;
    }
    for (i = 0; i < *(signed char *)(param_1 + param_3 + 0x36); i++) {
        if (func_ov006_020d7544(param_1, param_3, i) == 0) {
            func_0202f64c(&a[i]);
            func_0202f64c(&b[i]);
        } else {
            v1 = func_ov006_020d7594(param_1, param_3, *(signed char *)(param_1 + param_3 + 0x34), i);
            v2 = func_ov006_020d7594(param_1, param_3, *(signed char *)(param_1 + param_3 + 0x32), i);
            func_ov006_020d759c(param_1, param_2, param_3, i, param_4);
            func_02014ddc(a[i], v1 << 12);
            if (param_4 != 0) {
                func_02014ddc(a[i], v2 << 12);
            }
            if (param_3 == 0 && bit == 0) {
                func_02014ddc(b[i], v1 << 12);
                if (param_4 != 0) {
                    func_02014ddc(b[i], v2 << 12);
                }
            }
        }
    }
    func_ov006_020d7c8c((int)param_1, *(int *)(param_1 + 0x418), *(int *)(param_1 + 0x41c), ((signed char *)param_1 + param_3)[0x32], ((signed char *)param_1 + param_3)[0x36]);
}

extern void func_02010b7c(void);
extern int func_02010b60(void);
extern int func_02010b64(void);
extern int func_02015154(void);
extern int func_0202cb64(void);
extern void func_0202f64c(int *);
extern void func_02014ddc(int, int);
extern int func_ov006_020d7594(unsigned char *, int, int, int);
extern void func_ov006_020d759c(unsigned char *, int, int, int, int);

int func_ov006_020d79b4(unsigned char *param_1, int param_2, signed char param_3, int param_4, unsigned char param_5, int param_6)
{
    unsigned char bit;
    signed char i;
    int *a;
    int *b;
    signed char *q;
    int tmp;
    short local_c;
    int r0;
    int r7;
    int x;
    int w;

    bit = param_1[0x25] & 1;
    if (param_4 == 0) {
        func_02010b7c();
        if (func_02015154() != 0) {
            func_02010b7c();
            if (func_0202cb64() == 0) {
                goto cont;
            }
        }
    }
    if (param_4 == 0 || param_5 != 0) {
        return 1;
    }
cont:
    func_02010b7c();
    r7 = (short)func_02010b60();
    func_02010b7c();
    r0 = (short)func_02010b64();
    x = (short)(r7 - *(short *)(param_1 + 0x3e2));
    if (param_4 == 0) {
        if (r0 < 0x26 || r0 > 0x9a) {
            return 1;
        }
    }
    *(short *)(param_1 + 0x3e6) = (short)(x << 2);
    if (param_4 != 0) {
        *(short *)(param_1 + 0x3e6) = (short)param_6;
    }
    if (*(short *)(param_1 + 0x3e6) >= (param_3 << 7)) {
        *(short *)(param_1 + 0x3e6) = (short)(param_3 << 7);
    }
    x = (param_3 - *(signed char *)(param_1 + bit + 0x36) + 1) << 7;
    if (*(short *)(param_1 + 0x3e6) <= x) {
        *(short *)(param_1 + 0x3e6) = (short)x;
    }
    local_c = *(short *)(param_1 + 0x3e6) / 4;
    a = (int *)(param_1 + 0x438 + bit * 0x84);
    b = (int *)(param_1 + 0x540 + bit * 0x84);
    q = (signed char *)param_1 + bit;
    for (i = 0; i < q[0x36]; i++) {
        r0 = func_ov006_020d7594(param_1, bit, param_3, i);
        r7 = *(short *)(param_1 + 0x3e6) + r0;
        if (r7 < -128 || r7 > 384) {
            func_0202f64c(&a[i]);
            func_0202f64c(&b[i]);
        } else {
            func_ov006_020d759c(param_1, param_2, bit, i, 1);
            tmp = r7 << 12;
            func_02014ddc(a[i], tmp);
            if (b[i] != 0) {
                func_02014ddc(b[i], tmp);
            }
        }
    }
    w = *(int *)(param_1 + 0x660);
    func_02014ddc(w, (128 - (short)((128 - local_c) % 32)) << 12);
    return 0;
}

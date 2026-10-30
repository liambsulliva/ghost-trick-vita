extern void func_0202f64c(int *);
extern void func_020312a4(int);
extern int func_0207dc50();
extern int func_ov006_020d809c();
extern int func_ov006_020d813c();
extern void func_02014de4(int, int);
extern void func_ov006_020d7e88(int, short, short, short, short);
extern void func_ov006_020d7704(unsigned char *, int, int, int);
extern void func_ov006_020d7108(unsigned char *, int, int);

void func_ov006_020d6f88(unsigned char *param_1, int param_2, unsigned char param_3)
{
    int bit;
    int *a;
    int *b;
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned int m;

    bit = (unsigned char)(param_1[0x25] & 1);
    if (param_1[0x25] < 2) {
        if (*(int *)(param_1 + 0x65c) == 0) {
            *(int *)(param_1 + 0x65c) = func_0207dc50(2, 0xe, 0, ((unsigned int)0x020d7dc5u), 0);
            for (i = 0; i < 2; i++) {
                func_ov006_020d7e88(*(int *)(param_1 + 0x65c) + i * 0xe, (short)(i << 7), 0, (short)((i << 7) + 0x80), 0x1c);
            }
        }
        if (*(int *)(param_1 + 0x414) == 0 && *(int *)(param_1 + 0x20) == 0) {
            *(int *)(param_1 + 0x414) = func_ov006_020d809c(0x8002, 0x8003, 0x8003, 0xac, 0xa0, 0xf8, 0xbc);
        }
        if (*(int *)(param_1 + 0x660) == 0) {
            *(int *)(param_1 + 0x660) = func_ov006_020d813c(0xaf, 6, 2);
            func_02014de4(*(int *)(param_1 + 0x660), 0x5e000);
        }
        a = (int *)(param_1 + 0x438 + (1 - bit) * 0x84);
        b = (int *)(param_1 + 0x540 + (1 - bit) * 0x84);
        for (j = 0; j < 0x21; j++) {
            func_0202f64c(a + j);
            func_0202f64c(b + j);
        }
    } else {
        if (*(int *)(param_1 + 0x65c) != 0) {
            func_020312a4(*(int *)(param_1 + 0x65c));
            *(int *)(param_1 + 0x65c) = 0;
        }
        func_0202f64c((int *)(param_1 + 0x660));
        for (k = 0; k < 0x21; k++) {
            for (m = 0; m < 2; m++) {
                func_0202f64c((int *)(param_1 + 0x438 + m * 0x84) + k);
                func_0202f64c((int *)(param_1 + 0x540 + m * 0x84) + k);
            }
        }
    }
    func_ov006_020d7704(param_1, param_2, bit, param_3);
    func_0202f64c((int *)(param_1 + 0x434));
    func_ov006_020d7108(param_1, param_2, 1);
}

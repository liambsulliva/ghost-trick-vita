extern void func_0202f64c(int *);
extern void func_020312a4(void *);
extern void func_0201003c(void);
extern void func_020159a0(void);

void func_ov006_020d7240(unsigned char *param_1)
{
    unsigned int i;
    unsigned int j;
    unsigned int k;

    func_0202f64c((int *)(param_1 + 0x410));
    func_0202f64c((int *)(param_1 + 0x414));
    func_0202f64c((int *)(param_1 + 0x418));
    func_0202f64c((int *)(param_1 + 0x41c));
    func_0202f64c((int *)(param_1 + 0x420));
    func_0202f64c((int *)(param_1 + 0x424));
    func_0202f64c((int *)(param_1 + 0x428));
    func_0202f64c((int *)(param_1 + 0x42c));
    func_0202f64c((int *)(param_1 + 0x430));
    func_0202f64c((int *)(param_1 + 0x434));
    for (i = 0; i < 0x21; i++) {
        for (j = 0; j < 2; j++) {
            func_0202f64c((int *)(param_1 + 0x438 + j * 0x84 + i * 4));
            func_0202f64c((int *)(param_1 + 0x540 + j * 0x84 + i * 4));
        }
    }
    for (k = 0; k < 5; k++) {
        func_0202f64c((int *)(param_1 + 0x648 + k * 4));
    }
    func_0202f64c((int *)(param_1 + 0x660));
    if (*(void **)(param_1 + 0x65c) != 0) {
        func_020312a4(*(void **)(param_1 + 0x65c));
        *(void **)(param_1 + 0x65c) = 0;
    }
    func_0201003c();
    func_020159a0();
}

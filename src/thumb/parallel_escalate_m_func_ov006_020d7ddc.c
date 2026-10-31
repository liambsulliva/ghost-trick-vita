extern void *func_02010b7c(void);
extern int func_02015154(void *);
extern int func_0202cb64(void *);
extern int func_02010b60(void *);
extern int func_02010b64(void *);
extern unsigned int func_02015188(void *);

void func_ov006_020d7ddc(unsigned char *param_1)
{
    int a;
    int b;

    param_1[0] = 0;
    if (func_02015154(func_02010b7c()) == 0) {
        goto fail;
    }
    if (func_0202cb64(func_02010b7c()) != 0) {
        goto fail;
    }
    a = func_02010b60(func_02010b7c());
    b = func_02010b64(func_02010b7c());
    if (a < *(short *)(param_1 + 6)) {
        goto fail;
    }
    if (a > *(short *)(param_1 + 0xa)) {
        goto fail;
    }
    if (b < *(short *)(param_1 + 8)) {
        goto fail;
    }
    if (b > *(short *)(param_1 + 0xc)) {
        goto fail;
    }
    param_1[0] = 1;
fail:
    param_1[2] = 0;
    if (param_1[0] == param_1[1]) {
        *(signed char *)(param_1 + 4) = *(signed char *)(param_1 + 4) - 1;
        if (*(signed char *)(param_1 + 4) == 0 && param_1[5] != 0) {
            param_1[4] = 4;
            param_1[2] = param_1[0];
        } else {
            param_1[2] = 0;
        }
        param_1[3] = 0;
    } else {
        if (param_1[0] != 0 && func_02015188(func_02010b7c()) != 0) {
            param_1[4] = 0x1e;
            param_1[2] = param_1[0];
            param_1[5] = 1;
            param_1[3] = 1;
        } else {
            param_1[5] = 0;
            param_1[3] = 0;
        }
    }
    param_1[1] = param_1[0];
}

typedef struct {
    unsigned char value;
} Opt02007038;

extern Opt02007038 data_02080e80;
extern char data_020b18d8;
extern int func_02001fbc();
extern int func_02002108();
extern int func_020021e8();
extern int func_02002794();

int func_02007038(unsigned int param_1, int param_2, unsigned int param_3)
{
    Opt02007038 opt;
    unsigned int flags;
    int result;

    opt = data_02080e80;
    flags = *(unsigned int *)(*((int *)&data_020b18d8) + 0x15a4);
    if ((flags & 0x80000000) != 0 && (param_3 & 0x80) != 0) {
        param_3 = param_3 & 0x7f;
        *(unsigned int *)(*((int *)&data_020b18d8) + 0x15a4) = flags | 0x40000000;
    }
    if ((param_3 & 1) != 0) {
        result = func_02001fbc(param_1, param_2, param_3, &opt);
    } else if ((param_3 & 8) != 0) {
        result = func_02002108(param_1, param_2, param_3);
    } else if ((param_3 & 2) != 0) {
        result = func_020021e8(param_1, param_2, param_3);
    } else if ((param_3 & 4) != 0) {
        result = func_02002794(param_1, param_2);
    }
    if ((unsigned int)(result + 3) <= 1) {
        result = -3;
    }
    return result;
}

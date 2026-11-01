typedef struct {
    unsigned char value;
} Opt;

extern const Opt data_02080e7c;
extern int func_02002900(unsigned int param_1, unsigned int param_2, Opt *opt);
extern unsigned int func_020029e8(unsigned int param_1, unsigned int param_2);
extern int func_02002a88(unsigned int param_1, unsigned int param_2);
extern void func_02002b88(unsigned int param_1);

int func_020070b0(unsigned int param_1, unsigned int param_2)
{
    int ret;
    Opt opt;

    ret = 0;
    opt = data_02080e7c;
    if (param_2 & 1) {
        ret = func_02002900(param_1, param_2, &opt);
    } else if (param_2 & 8) {
        ret = func_020029e8(param_1, param_2);
    } else if (param_2 & 2) {
        ret = func_02002a88(param_1, param_2);
    } else if (param_2 == 4) {
        func_02002b88(param_1);
    }
    if ((unsigned int)(ret + 3) <= 1) {
        ret = -3;
    }
    return ret;
}

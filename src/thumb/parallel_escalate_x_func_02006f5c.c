typedef struct {
    unsigned char value;
} Opt02006f5c;

extern Opt02006f5c data_02080e78;
extern int func_020018f0();
extern int func_020019d4();
extern int func_02001b00();
extern int func_02001c1c(unsigned int param_1);

int func_02006f5c(unsigned int param_1, unsigned int param_2)
{
    Opt02006f5c opt;
    int result;

    result = 0;
    opt = data_02080e78;
    if ((param_2 & 1) != 0) {
        result = func_020018f0(param_1, param_2, &opt);
    } else if ((param_2 & 8) != 0) {
        result = func_020019d4(param_1, param_2);
    } else if ((param_2 & 2) != 0) {
        result = func_02001b00(param_1, param_2);
    } else if ((param_2 & 4) != 0) {
        result = func_02001c1c(param_1);
    }
    if ((unsigned int)(result + 3) <= 1) {
        result = -3;
    }
    return result;
}

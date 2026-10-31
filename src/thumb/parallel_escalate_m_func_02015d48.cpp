class S02015d48 {
public:
    virtual int method();
};

extern "C" {
extern int (S02015d48::* const data_020848ac[])();
extern unsigned short *func_02015dac(void *object);
extern int func_02016364(unsigned char *object);

int func_02015d48(unsigned char *param_1, void *param_2)
{
    unsigned short *cur;
    unsigned int op;
    int result;

    *(void **)param_1 = param_2;
    if (param_2 == 0) {
        return 1;
    }
    cur = func_02015dac(param_2);
    *(unsigned short **)(param_1 + 8) = cur;
    if (*cur != 0xfffe) {
        op = *cur - 0xff00;
        *(unsigned int *)(param_1 + 4) = op;
        if (op >= 0x31) {
            param_1[0x10] = 1;
            return 1;
        }
        result = (((S02015d48 *)param_1)->*data_020848ac[op])();
    } else {
        result = func_02016364(param_1);
    }
    param_1[0x10] = 0;
    return result;
}
}

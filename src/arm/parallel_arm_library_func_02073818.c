extern unsigned int func_02075cc4();
extern unsigned int func_02075c98();
extern void func_02075cd8();
extern void func_02075cac();

int func_02073818(unsigned int param_1,unsigned int *param_2,void (*param_3)(),unsigned int param_4)
{
    unsigned int saved;

    if (param_1 != ((unsigned short *)param_2)[2]) {
        return -2;
    }
    if (param_4 != 0) {
        saved = func_02075cc4();
    } else {
        saved = func_02075c98();
    }
    ((unsigned short *)param_2)[2] = 0;
    if (param_3 != 0) {
        param_3();
    }
    *param_2 = 0;
    if (param_4 != 0) {
        func_02075cd8(saved);
    } else {
        func_02075cac(saved);
    }
    return 0;
}

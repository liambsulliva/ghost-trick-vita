struct State {
    unsigned int before[2];
    unsigned int *first;
    unsigned int *last;
};
extern struct State data_020d1344;
extern int func_02075c98();
extern int func_02075cac();

void func_02076b28(unsigned int *param_1)

{
    unsigned int saved;

    saved = func_02075c98();
    if (data_020d1344.last == (unsigned int *)0x0) {
        data_020d1344.first = param_1;
        data_020d1344.last = param_1;
    }
    else {
        *data_020d1344.last = (unsigned int)(param_1);
        data_020d1344.last = param_1;
    }
    *param_1 = 0;
    func_02075cac(saved);
    return;
}

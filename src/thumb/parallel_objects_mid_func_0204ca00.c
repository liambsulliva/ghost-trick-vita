struct State {
    unsigned int before;
    unsigned char *object;
};
extern struct State data_020ceaa4;
extern int func_0202f644();
extern unsigned char * func_02047198();

unsigned char * func_0204ca00(unsigned char *param_1)

{
    if (data_020ceaa4.object == param_1) {
        data_020ceaa4.object = 0;
    }
    func_02047198(param_1);
    func_0202f644(param_1);
    return param_1;
}

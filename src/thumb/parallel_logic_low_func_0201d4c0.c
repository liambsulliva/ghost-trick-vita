struct State {
    unsigned int first;
    int count;
};
extern struct State data_020b9d14;
extern unsigned char * func_0202f5e4();
extern int func_0202f644();

unsigned char * func_0201d4c0(unsigned char *param_1)

{
    if (data_020b9d14.count != 0) {
        data_020b9d14.count = data_020b9d14.count + -1;
    }
    func_0202f5e4(param_1);
    func_0202f644(param_1);
    return param_1;
}

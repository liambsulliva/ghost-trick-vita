extern char data_020ce320;
extern int func_0202f644();
extern unsigned char * func_02040dbc();

unsigned char * func_020416b0(unsigned char *param_1)

{
    *((char *)&data_020ce320) = *((char *)&data_020ce320) + -1;
    func_02040dbc(param_1);
    func_0202f644(param_1);
    return param_1;
}

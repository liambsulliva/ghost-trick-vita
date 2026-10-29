extern char data_020ce320;
extern unsigned char * func_02040dbc();

unsigned char * func_020416d0(unsigned char *param_1)

{
    *((char *)&data_020ce320) = *((char *)&data_020ce320) + -1;
    func_02040dbc(param_1);
    return param_1;
}

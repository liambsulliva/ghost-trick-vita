extern char data_0209d71c;
extern char data_0209d720;
extern char data_0209d728;
extern char data_0209d72c;
extern char data_020ad40c;
extern int func_0200da04();
extern int func_0201003c();
extern int func_02015c8c();
extern int func_0202f5e4();
extern int func_0202f644();
extern int func_02042774();
extern int func_0207dea4();

unsigned int * func_02054e3c(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020ad40c);
    func_0201003c();
    func_02015c8c();
    if (param_1[10] != 0) {
        func_0207dea4(param_1[10],200,8,((unsigned int)0x02056c8du));
        param_1[10] = 0;
    }
    func_0200da04(((unsigned int)&data_0209d71c),((unsigned int)&data_0209d720),((unsigned int)&data_0209d728),((unsigned int)&data_0209d72c));
    func_02042774(param_1 + 0xd);
    func_0202f5e4(param_1);
    func_0202f644(param_1);
    return param_1;
}

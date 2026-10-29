extern int func_0202323c();
extern int func_0207dd08();

unsigned char * func_02023240(unsigned char *param_1)

{
    func_0207dd08((int)param_1,2,0x34,((unsigned char *)0x02022ee1u),((int)0x02023275u));
    func_0202323c(param_1,(int)(param_1 + 0x34));
    func_0202323c(param_1 + 0x34,(int)param_1);
    return param_1;
}

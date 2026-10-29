extern int func_0202a29c();
extern int func_0202a49c();
extern int func_0202a5dc();
extern int func_0202a714();

unsigned int
func_0202a450(unsigned char *param_1,unsigned char *param_2,unsigned int param_3,unsigned int param_4)

{
    if ((unsigned char)param_1[0x24] == 2) {
        return ((unsigned int)0x0000ffffu);
    }
    func_0202a29c((int)param_1,(int)param_2,(unsigned int)(unsigned char)param_1[0x24],param_4);
    func_0202a714((int)param_1,0,0);
    if (param_2 != (unsigned char *)(unsigned int)*(unsigned short *)(param_1 + 0x3a)) {
        func_0202a5dc((int)param_1,0);
        param_1[0x39] = 6;
        func_0202a49c(param_1,0,param_2);
    }
    return 0;
}

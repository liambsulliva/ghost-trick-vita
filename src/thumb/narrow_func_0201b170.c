extern char data_0201b155;
extern char data_020a3914;
extern int func_02016c4c();
extern int func_0207dd08();

unsigned int * func_0201b170(unsigned int *param_1)

{
    func_02016c4c(param_1);
    *param_1 = ((unsigned int)&data_020a3914);
    *(unsigned short *)(param_1 + 0x1d) = 0;
    *(unsigned char *)((int)param_1 + 0x76) = 0;
    *(unsigned short *)(param_1 + 0x20) = 0;
    *(unsigned short *)((int)param_1 + 0x82) = 0;
    *(unsigned short *)(param_1 + 0x21) = 0;
    *(unsigned char *)((int)param_1 + 0x86) = 0;
    *(unsigned char *)((int)param_1 + 0x87) = 0;
    *(unsigned short *)(param_1 + 0x22) = 0;
    *(unsigned short *)((int)param_1 + 0x8a) = 0;
    *(unsigned short *)(param_1 + 0x23) = 0;
    param_1[0x1a] = 0;
    param_1[0x1b] = 0;
    param_1[0x1c] = 0;
    param_1[0x1e] = 0;
    param_1[0x1f] = 0;
    func_0207dd08((int)param_1 + 0x8e,8,0xe,((unsigned char *)&data_0201b155),((int)0x0201b16du));
    return param_1;
}

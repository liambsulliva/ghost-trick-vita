typedef struct {
    unsigned short value;
    unsigned short pad;
    int rest;
} Rec0202ab38;

extern Rec0202ab38 data_02080e84[];
extern Rec0202ab38 data_02080e86[];
extern void func_02007250(int a,int b,int c,int d,int e,int f,int g,short h,signed char i,int j,unsigned short k,int l);

void func_0202ab38(int param_1,int param_2,int param_3,int param_4,unsigned short param_5)
{
    int index;

    if (*(unsigned char *)(param_1 + 0x24) != 1) {
        index = *(int *)(param_1 + param_2 * 0xc + 0x1ec);
        func_02007250(data_02080e86[index].value,data_02080e84[index].value,0,0x2800,0,0,0,param_3 >> 1,param_4,0,param_5,param_2);
        *(short *)(param_1 + 0x248) = param_3;
        *(short *)(param_1 + 0x24a) = param_4;
    }
}

extern unsigned char *func_0200883c();
extern int *func_02008824();
extern unsigned int func_0201e97c();

typedef struct Obj {
    char pad[0xc];
    unsigned char lo:2;
    unsigned char big:1;
    unsigned char b3:1;
    unsigned char hi:4;
    unsigned char pad_d;
    unsigned short id;
} Obj;

void func_0200b020(Obj *param_1, int param_2, int param_3)
{
    unsigned int result;

    param_1->id = param_2;
    param_1->hi = (unsigned char)param_3;
    result = func_0201e97c(func_02008824(func_0200883c()), 0, param_2);
    if (result > 0x20) {
        param_1->big = 1;
    } else {
        param_1->big = 0;
    }
}

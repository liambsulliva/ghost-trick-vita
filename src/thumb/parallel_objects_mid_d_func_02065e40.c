typedef struct {
    int first;
    int second;
} Pair;

extern char data_0209e150;
extern char data_0209e15c;
extern char data_0209e168;
extern char data_0209e174;
extern char data_0209e180;
extern char data_0209e18c;
extern char data_0209e198;
extern char data_0209e1ac;
extern char data_020b158c;
extern char data_020b1594;
extern char data_020b159c;
extern char data_020b15a4;
extern int func_020527c8();
extern int func_020121e4();
extern int func_0200b374();
extern int func_02008a64();
extern int func_02008a30();
extern int func_02012f14();
extern int func_02012ea8();
extern int func_0200b9b8();
extern int func_02008a00();
extern int func_020089fc();

void func_02065e40(int param_1,int param_2)
{
    unsigned char auStack_4c[56];
    unsigned char auStack_84[56];
    unsigned char auStack_bc[56];
    unsigned char auStack_f4[56];
    unsigned char auStack_12c[56];
    unsigned char auStack_164[56];
    unsigned char auStack_19c[56];
    unsigned char auStack_1d4[56];

    func_020527c8();
    func_020121e4(auStack_4c,param_1,(int)&data_0209e150,param_1 + 0xcc,1,0);
    func_02008a00(param_2,auStack_4c);
    func_020089fc(auStack_4c);
    func_0200b374(auStack_84,param_1,(int)&data_0209e15c,param_1 + 0xc4,1,0);
    func_02008a00(param_2,auStack_84);
    func_020089fc(auStack_84);
    func_02008a64(auStack_bc,param_1,(int)&data_0209e168,param_1 + 0xc8,1,0);
    func_02008a00(param_2,auStack_bc);
    func_020089fc(auStack_bc);
    func_02008a30(auStack_f4,param_1,(int)&data_0209e174,param_1 + 0xce,1,0);
    func_02008a00(param_2,auStack_f4);
    func_020089fc(auStack_f4);
    func_02012f14(auStack_12c,param_1,(int)&data_0209e180,param_1 + 0xb4,1,0);
    func_02008a00(param_2,auStack_12c);
    func_020089fc(auStack_12c);
    func_02008a30(auStack_164,param_1,(int)&data_0209e18c,param_1 + 0xc0,1,0);
    func_02008a00(param_2,auStack_164);
    func_020089fc(auStack_164);
    func_02012ea8(auStack_19c,param_1,(int)&data_0209e198,param_1 + 0xd4,1,0);
    func_02008a00(param_2,auStack_19c);
    func_020089fc(auStack_19c);
    func_0200b9b8(auStack_1d4,param_1,2,(int)&data_0209e1ac,*(Pair *)&data_020b158c,*(Pair *)&data_020b1594,0x21,*(Pair *)&data_020b159c,*(Pair *)&data_020b15a4);
    func_02008a00(param_2,auStack_1d4);
    func_020089fc(auStack_1d4);
    return;
}

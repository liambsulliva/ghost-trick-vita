class Object02062e50_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int argument0,int argument1);
};

class Object02062e50_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
};

extern "C" {
extern char data_0209de5c;
extern char data_020af8e4;
typedef int code(...);
extern int func_02007b70(...);
extern int func_02007bd8(...);
extern int func_0200ee2c(...);
extern int func_0200f70c(...);
extern int func_020569f0(...);

int * func_02062e50(int *param_1)

{
    int *piVar1;
    int iVar2;

    func_020569f0();
    *param_1 = ((int)&data_020af8e4);
    func_0200f70c(param_1 + 0x1f);
    *(unsigned char *)((int)param_1 + 0x92) = 0;
    *(unsigned char *)((int)param_1 + 0x93) = 0;
    *(unsigned short *)(param_1 + 0x25) = 0x540;
    func_02007bd8();
    piVar1 = (int *)func_02007b70();
    iVar2 = ((Object02062e50_0 *)piVar1)->slot2((int)(0x88),(int)(((unsigned int)&data_0209de5c)));
    param_1[0x26] = iVar2;
    *(unsigned short *)(param_1 + 0x24) = 0;
    *(unsigned char *)(param_1 + 0x11) = 1;
    ((Object02062e50_1 *)param_1)->slot10();
    func_0200ee2c(param_1,0x560);
    return param_1;
}
}

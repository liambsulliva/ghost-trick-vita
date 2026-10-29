class Object02054d44_0 {
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
typedef int code(...);
extern unsigned char * func_0202f524(...);
extern unsigned char * func_02042718(...);

int * func_02054d44(int *param_1)

{
    int iVar1;

    func_0202f524((unsigned char *)param_1);
    iVar1 = ((int)0x020ad40cu);
    param_1[10] = 0;
    *param_1 = iVar1;
    param_1[0xb] = 0;
    param_1[0xc] = 0;
    func_02042718((unsigned char *)(param_1 + 0xd));
    *(unsigned char *)(param_1 + 0x22) = *(unsigned char *)(param_1 + 0x22) & ~0x1;
    *(unsigned char *)(param_1 + 0x22) = *(unsigned char *)(param_1 + 0x22) | 2;
    *(unsigned char *)((int)param_1 + 0x89) = 0;
    *(unsigned char *)((int)param_1 + 0x8a) = 0;
    *(unsigned char *)((int)param_1 + 0x8b) = 0;
    *(unsigned short *)(param_1 + 0x23) = 0;
    *(unsigned short *)((int)param_1 + 0x8e) = 0;
    *(unsigned short *)((int)param_1 + 0x92) = 1;
    param_1[0x25] = 0;
    param_1[0x26] = 0;
    param_1[0x27] = 0;
    *(unsigned char *)(param_1 + 0x28) = 0;
    *(unsigned short *)((int)param_1 + 0xa2) = 0;
    *(unsigned short *)(param_1 + 0x29) = 0;
    param_1[0x2a] = 0;
    ((Object02054d44_0 *)(param_1))->slot10();
    return param_1;
}
}

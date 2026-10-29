class Object0202dab4_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

extern "C" {
typedef int code(...);
extern int func_02007b70(...);
extern unsigned char * func_02007bd8(...);
extern int func_0202b56c(...);
extern unsigned char * func_0202cdc4(...);
extern int func_0202db3c(...);
extern int func_0207dea4(...);

unsigned int * func_0202dab4(unsigned int *param_1)

{
    unsigned char *puVar1;
    int iVar2;
    int *piVar3;
    unsigned int uVar4;

    *param_1 = ((unsigned int)0x020a5240u);
    if (param_1[0x7a] != 0) {
        puVar1 = func_02007bd8();
        iVar2 = func_02007b70(puVar1);
        if (iVar2 != 0) {
            puVar1 = func_02007bd8();
            piVar3 = (int *)func_02007b70(puVar1);
            ((Object0202dab4_0 *)(piVar3))->slot4((int)(param_1[0x7a]));
            param_1[0x7a] = 0;
        }
    }
    func_0202b56c((int)param_1);
    if (param_1[0x7c] != 0) {
        for (uVar4 = 0; uVar4 < (unsigned int)param_1[0x51]; uVar4 = uVar4 + 1) {
            func_0202db3c(param_1[0x7c] + uVar4 * 0x6c);
        }
        if (param_1[0x7c] != 0) {
            func_0207dea4(param_1[0x7c],0x6c,8,((unsigned char *)0x0202dbddu));
            param_1[0x7c] = 0;
        }
    }
    func_0202cdc4((unsigned char *)param_1);
    return param_1;
}
}

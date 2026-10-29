class Object0202c850_0 {
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
extern unsigned char * func_0202b400(...);
extern int func_0202b56c(...);

unsigned int * func_0202c850(unsigned int *param_1)

{
    unsigned char *puVar1;
    int iVar2;
    int *piVar3;

    *param_1 = ((unsigned int)0x020a50fcu);
    func_0202b56c((int)param_1);
    if (param_1[0x2b] != 0) {
        puVar1 = func_02007bd8();
        iVar2 = func_02007b70(puVar1);
        if (iVar2 != 0) {
            puVar1 = func_02007bd8();
            piVar3 = (int *)func_02007b70(puVar1);
            ((Object0202c850_0 *)(piVar3))->slot4((int)(param_1[0x2b]));
            param_1[0x2b] = 0;
        }
    }
    func_0202b400((unsigned char *)param_1);
    return param_1;
}
}

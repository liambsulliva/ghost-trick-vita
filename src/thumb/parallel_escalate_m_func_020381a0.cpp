class Deletable {
public:
    virtual ~Deletable();
};

extern "C" {
extern unsigned char *func_02010b7c();
extern int func_0202f17c(...);
extern int func_0202f64c(...);
extern int func_02031270(...);
extern int func_02038200(...);
extern unsigned char *func_02039d2c(...);
extern unsigned char *func_0203a408(...);

unsigned int *func_020381a0(unsigned int *param_1)
{
    unsigned char *puVar1;

    puVar1 = (unsigned char *)*param_1;
    if (puVar1 == (unsigned char *)0x0) {
        goto second;
    }
    if (puVar1 == (unsigned char *)0x0) {
        goto clear1;
    }
    func_02039d2c(puVar1);
    func_02031270((int)puVar1);
clear1:
    *param_1 = 0;
second:
    puVar1 = (unsigned char *)param_1[3];
    if (puVar1 == (unsigned char *)0x0) {
        goto third;
    }
    if (puVar1 == (unsigned char *)0x0) {
        goto clear2;
    }
    func_0203a408(puVar1);
    func_02031270((int)puVar1);
clear2:
    param_1[3] = 0;
third:
    func_0202f64c((unsigned char *)(param_1 + 1));
    if ((unsigned char *)param_1[4] != (unsigned char *)0x0) {
        func_02038200((unsigned char *)param_1[4]);
        puVar1 = func_02010b7c();
        func_0202f17c((int)puVar1, param_1[4]);
        delete (Deletable *)param_1[4];
    }
    return param_1;
}
}

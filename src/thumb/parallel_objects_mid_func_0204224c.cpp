struct Flags {
    unsigned int flag : 1;
};
class Object0204224c_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual int slot3(int argument0);
};

extern "C" {
typedef int code(...);
extern int func_02007b70(...);
extern unsigned char * func_02007bd8(...);
extern int func_0204228c(...);
extern int func_02073174(...);

unsigned int * func_0204224c(int param_1)

{
    unsigned int *puVar1;
    int iVar2;
    unsigned char *puVar3;
    int *piVar4;

    puVar1 = *(unsigned int **)(param_1 + 4);
    if (((struct Flags *)puVar1)->flag == 1) {
        iVar2 = func_0204228c((unsigned char *)(puVar1 + 2));
        puVar3 = func_02007bd8();
        piVar4 = (int *)func_02007b70(puVar3);
        puVar1 = (unsigned int *)((Object0204224c_0 *)(piVar4))->slot3((int)(iVar2));
        func_02073174((unsigned int *)(*(int *)(param_1 + 4) + 8),(unsigned char *)puVar1);
    }
    else {
        puVar1 = puVar1 + 2;
    }
    return puVar1;
}
}

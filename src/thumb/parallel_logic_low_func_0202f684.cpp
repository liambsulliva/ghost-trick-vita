class Object0202f684_0 {
public:
    virtual int slot0();
};

extern "C" {
typedef int code(...);
extern int func_0201ee30(...);

unsigned int func_0202f684(int param_1)

{
    unsigned int *puVar1;
    unsigned int uVar2;

    puVar1 = (unsigned int *)func_0201ee30(param_1,((unsigned int)0x020bbbb4u));
    if (puVar1 != (unsigned int *)0x0) {
        return ((Object0202f684_0 *)(puVar1))->slot0();
    }
    return 0;
}
}

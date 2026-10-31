class Item {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual int v14();
};
class Owner {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0(int value, Item *item);
};
extern "C" {
extern int func_0204235c(unsigned short id);
extern void *func_0200b208(void);
extern Item *func_020575b8(void *object, int value);

void func_0206618c(char *param_1)
{
    Item *item;

    *(int *)(param_1 + 0xd0) = func_0204235c(*(unsigned short *)(param_1 + 0x88));
    item = func_020575b8(func_0200b208(), *(int *)(param_1 + 0xd0));
    *(unsigned char *)(param_1 + 0xcc) = 0;
    *(char **)(*(char **)(param_1 + 0xc8) + 0xb4) = param_1 + 0xcc;
    (*(Owner **)(param_1 + 0xc8))->va0(item->v14(), item);
    (*(Owner **)(param_1 + 0xc8))->v2c();
}
}

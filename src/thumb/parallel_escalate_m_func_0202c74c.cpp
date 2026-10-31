struct Entry {
    unsigned int low:1;
    unsigned int width:12;
    unsigned int height:12;
};
class Sprite {
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
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0(int value);
};
extern "C" {
extern struct Entry *func_0202c820(int value);
extern int func_0202c824(int value);
extern void func_02008398(int *vec, int x, int y, int z);
extern void func_020083b0(void *dst, int *src);
extern void func_0205765c(void *object, int a, int b, int c, int d);

void func_0202c74c(char *param_1, int param_2)
{
    int v20[3];
    int v2c[3];
    int v38[3];
    int v44[3];
    struct Entry *entry;

    *(int *)(param_1 + 0x1c0) = param_2;
    entry = func_0202c820(param_2);
    *(int *)(param_1 + 0x98) = entry->width << 12;
    *(int *)(param_1 + 0x9c) = entry->height << 12;
    func_02008398(v20, 0, 0, 0x1000);
    func_020083b0(param_1 + 0xa0, v20);
    func_02008398(v2c, 0, *(int *)(param_1 + 0x9c), 0x1000);
    func_020083b0(param_1 + 0xac, v2c);
    func_02008398(v38, *(int *)(param_1 + 0x98), *(int *)(param_1 + 0x9c), 0x1000);
    func_020083b0(param_1 + 0xb8, v38);
    func_02008398(v44, *(int *)(param_1 + 0x98), 0, 0x1000);
    func_020083b0(param_1 + 0xc4, v44);
    ((Sprite *)(param_1 + 0xf4))->vb0(func_0202c824(*(int *)(param_1 + 0x1c0)));
    func_0205765c(param_1 + 0xf4, 0, 0, *(int *)(param_1 + 0x98), *(int *)(param_1 + 0x9c));
}
}

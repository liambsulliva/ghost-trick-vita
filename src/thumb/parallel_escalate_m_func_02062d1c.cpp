class Viewer {
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
    virtual void v84(unsigned short value);
};
typedef struct {
    unsigned short v[5];
} Row;
extern "C" {
extern const Row data_0209dd2c;
extern const Row data_0209dd14;
extern void func_02062b88(char *object, int index);
extern void func_0204c540(Viewer *viewer, unsigned short value);

void func_02062d1c(char *param_1, int param_2)
{
    Row b;
    Row a;

    if (param_2 < 0) {
        param_2 = 0;
    }
    if (param_2 > 4) {
        param_2 = 4;
    }
    b = data_0209dd2c;
    a = data_0209dd14;
    func_02062b88(param_1, param_2);
    func_0204c540(*(Viewer **)(param_1 + 0xf4), a.v[param_2]);
    (*(Viewer **)(param_1 + 0xf4))->v84(b.v[param_2]);
}
}

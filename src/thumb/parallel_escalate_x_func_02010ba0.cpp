class V02010ba0 {
public:
    virtual int v00();
    virtual int v04();
    virtual int v08();
    virtual int v0c();
    virtual int v10();
    virtual int v14();
    virtual int v18();
    virtual int v1c();
    virtual int v20();
    virtual int v24();
    virtual int v28();
    virtual int v2c();
    virtual int v30();
    virtual int v34();
    virtual int v38();
    virtual int v3c();
    virtual int v40();
    virtual int v44();
    virtual int v48();
    virtual int v4c();
    virtual int v50();
    virtual int v54();
    virtual int v58();
    virtual int v5c();
    virtual int v60();
    virtual int v64();
    virtual int v68();
    virtual int v6c();
    virtual int v70();
    virtual int v74();
    virtual int v78();
    virtual int v7c();
    virtual int v80();
    virtual int v84();
    virtual int v88();
    virtual int v8c();
    virtual int v90();
    virtual int v94();
    virtual int v98();
    virtual int v9c();
    virtual int va0();
    virtual int va4();
    virtual int va8();
    virtual int vac();
    virtual int vb0();
    virtual int vb4();
    virtual int vb8();
    virtual int vbc();
    virtual int vc0();
    virtual int vc4();
    virtual int vc8();
    virtual int vcc();
    virtual int vd0();
    virtual int vd4();
    virtual int vd8();
    virtual void vdc(int a, int b);
    virtual int ve0();
    virtual int ve4();
    virtual int ve8();
    virtual int vec();
};

extern "C" {
extern void func_02010c18(V02010ba0 *object, int flag);
extern void func_02010c20(V02010ba0 *object, int flag);

void func_02010ba0(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    unsigned int i;
    int base;
    int id;

    for (i = 0; i < 8; i++) {
        base = param_1 + i * 4;
        if (*(V02010ba0 **)(base + 0xdfc) != 0) {
            id = (*(V02010ba0 **)(base + 0xdfc))->ve8();
            if (id == param_2) {
                (*(V02010ba0 **)(base + 0xdfc))->vdc(param_3, param_4);
                switch (param_5) {
                case 0:
                default:
                    func_02010c18(*(V02010ba0 **)(base + 0xdfc), 0);
                    func_02010c20(*(V02010ba0 **)(base + 0xdfc), 0);
                    break;
                case 1:
                    func_02010c18(*(V02010ba0 **)(base + 0xdfc), 1);
                    func_02010c20(*(V02010ba0 **)(base + 0xdfc), 0);
                    break;
                case 2:
                    func_02010c18(*(V02010ba0 **)(base + 0xdfc), 0);
                    func_02010c20(*(V02010ba0 **)(base + 0xdfc), 1);
                    break;
                }
            }
        }
    }
}
}

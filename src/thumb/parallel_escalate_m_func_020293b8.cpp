class Object020293b8 {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(int size);
    char pad04[0x44];
    int field48;
    char pad4c[9];
    unsigned char field55;
    unsigned char field56;
    unsigned char field57;
    int pad58;
    int field5c;
};

extern "C" {
extern char data_020a4e24;
void func_02056978(Object020293b8 *object);

Object020293b8 *func_020293b8(Object020293b8 *object)
{
    func_02056978(object);
    *(void **)object = &data_020a4e24;
    object->field48 = 0;
    object->field55 = 1;
    object->field56 = 0;
    object->field57 = 0;
    object->field5c = -1;
    object->v84(0x5f0);
    return object;
}
}

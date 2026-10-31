class Obj194 {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33(int);
    virtual void v34();
    virtual void v35(int);
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46(int, int);
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65(int);
};

extern "C" {
extern char data_020cf724;
extern unsigned int func_0202ff58(Obj194 *obj);
extern Obj194 *func_0205693c(void *desc);
extern void func_020153d8(Obj194 *object, Obj194 *value);
extern int func_02009900(void);
extern void func_0202f7e8(int param_1, Obj194 *param_2, unsigned int param_3, int param_4);
extern void func_020660c4(Obj194 *obj);

#define CHILD(o) (*(Obj194 **)((char *)(o) + 0xd8))
#define COUNT(o) (*(unsigned int *)((char *)(o) + 0xdc))

void func_02067194(Obj194 *self)
{
    Obj194 *child;
    unsigned int count;

    if (func_0202ff58(self) != 0) {
        if (COUNT(self) == 0) {
            self->v65(0x2d);
        }
        if (COUNT(self) == 0) {
            child = func_0205693c(&data_020cf724);
            CHILD(self) = child;
            if (child != 0) {
                child->v33(0);
                CHILD(self)->v46(0x100000, 0xc0000);
                func_020153d8(CHILD(self), self);
                func_0202f7e8(func_02009900(), CHILD(self), 7, 0);
            }
        }
        count = COUNT(self);
        if (count >= 4 && count <= 0xf && (child = CHILD(self)) != 0) {
            child->v35((signed char)(((0xf - count) * 0x1f) / 0xb));
        }
        func_020660c4(self);
        COUNT(self) = COUNT(self) + 1;
    }
}
}

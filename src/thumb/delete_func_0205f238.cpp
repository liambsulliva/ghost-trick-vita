class Object0205f238_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

class Object0205f238_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

class Object0205f238_2 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

class Object0205f238_3 {
public:
    virtual ~Object0205f238_3();
};

class Object0205f238_4 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

class Object0205f238_5 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

class Object0205f238_6 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

class Object0205f238_7 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

extern "C" {
extern char data_020af5c0;
typedef int code(...);
extern int func_02007b70(...);
extern int func_02007bd8(...);
extern int func_02056c6c(...);

unsigned int * func_0205f238(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020af5c0);
    if (param_1[0x34] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object0205f238_0 *)(piVar2))->slot4((int)(param_1[0x34]));
            param_1[0x34] = 0;
        }
    }
    if (param_1[0x35] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object0205f238_1 *)(piVar2))->slot4((int)(param_1[0x35]));
            param_1[0x35] = 0;
        }
    }
    if (param_1[0x36] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object0205f238_2 *)(piVar2))->slot4((int)(param_1[0x36]));
            param_1[0x36] = 0;
        }
    }
    if ((int *)param_1[0x37] != (int *)0x0) {
        delete ((Object0205f238_3 *)(param_1[0x37]));
        param_1[0x37] = 0;
    }
    if (param_1[0x3c] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object0205f238_4 *)(piVar2))->slot4((int)(param_1[0x3c]));
            param_1[0x3c] = 0;
        }
    }
    if (param_1[0x39] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object0205f238_5 *)(piVar2))->slot4((int)(param_1[0x39]));
            param_1[0x39] = 0;
        }
    }
    if (param_1[0x3a] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object0205f238_6 *)(piVar2))->slot4((int)(param_1[0x3a]));
            param_1[0x3a] = 0;
        }
    }
    if (param_1[0x38] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object0205f238_7 *)(piVar2))->slot4((int)(param_1[0x38]));
            param_1[0x38] = 0;
        }
    }
    func_02056c6c(param_1);
    return param_1;
}
}

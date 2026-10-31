class Actor545c {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual int slot24();
    virtual int slot25();
};

struct Cmd545c {
    unsigned int vt;
    int value;
    unsigned char b8;
    unsigned char b9;
    unsigned char ba;
    unsigned char bb;
};

extern "C" {
extern char data_020cfe2c;
extern char data_020ce900;
extern char data_020cf388;
extern char data_020ca128;
extern char data_020cf778;
extern char data_020cf898;
extern char data_020cf538;
extern int func_0201572c(void);
extern void *func_02008444(void);
extern void func_02011404(void);
extern void func_02011044(void *object);
extern void func_0202fbb0(void *iterator);
extern void func_ov006_020d5628(Cmd545c *cmd);
extern Actor545c *func_0200eab4(void *iterator);
extern Actor545c *func_0200eab8(void *iterator);
extern int func_0200eac4(void *iterator);
extern void func_0200eabc(void *iterator, int value);
extern int func_02008828(void *object, void *kind);
extern int func_02010b54(void *object);
extern void func_0201001c(void *object);
extern void func_ov006_020d50f4(unsigned int *out, Cmd545c *cmd);
extern void func_ov006_020d5640(unsigned int *list, unsigned int *item);
extern void func_0203ed0c(unsigned int *item);

void func_ov006_020d545c(char *param_1)
{
    unsigned int it[4];
    Cmd545c cmd;
    unsigned int first[3];
    unsigned int second[3];
    unsigned int third[3];
    Actor545c *obj;

    if (func_0201572c() != 0) {
        return;
    }
    *(unsigned char *)(param_1 + 0x24) = 3;
    func_02008444();
    func_02011404();
    func_02011044(func_02008444());
    func_0202fbb0(it);
    while (func_0200eac4(it) != 0) {
        func_ov006_020d5628(&cmd);
        if (func_02008828(func_0200eab4(it), &data_020cfe2c) != 0 ||
            func_02008828(func_0200eab4(it), &data_020ce900) != 0 ||
            func_02008828(func_0200eab4(it), &data_020cf388) != 0 ||
            func_02008828(func_0200eab4(it), &data_020ca128) != 0 ||
            func_02008828(func_0200eab4(it), &data_020cf778) != 0) {
            if (func_02010b54(func_0200eab4(it)) != 0) {
                cmd.ba = 1;
                cmd.b8 = 0;
                cmd.b9 = 0;
                cmd.value = (int)func_0200eab8(it);
                func_ov006_020d50f4(first, &cmd);
                func_ov006_020d5640((unsigned int *)(param_1 + 0x668), first);
                func_0203ed0c(first);
                func_0200eab4(it)->slot11();
                func_0201001c(func_0200eab4(it));
                *(int *)(param_1 + 0x664) += 1;
            }
        } else if (func_02008828(func_0200eab4(it), &data_020cf898) != 0) {
            obj = func_0200eab8(it);
            if (obj->slot24() == 0 && obj->slot25() == 0) {
                cmd.ba = 0;
                cmd.b8 = 1;
                cmd.b9 = 0;
                cmd.value = (int)func_0200eab8(it);
                cmd.bb = func_02010b54(obj);
                func_ov006_020d50f4(second, &cmd);
                func_ov006_020d5640((unsigned int *)(param_1 + 0x668), second);
                func_0203ed0c(second);
                obj->slot23();
                *(int *)(param_1 + 0x664) += 1;
            }
        } else if (func_02008828(func_0200eab4(it), &data_020cf538) != 0) {
            obj = func_0200eab8(it);
            cmd.ba = 0;
            cmd.b8 = 0;
            cmd.b9 = 1;
            cmd.value = (int)func_0200eab8(it);
            func_ov006_020d50f4(third, &cmd);
            func_ov006_020d5640((unsigned int *)(param_1 + 0x668), third);
            func_0203ed0c(third);
            obj->slot12();
            *(int *)(param_1 + 0x664) += 1;
        }
        func_0200eabc(it, 0);
    }
}
}

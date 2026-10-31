class Unit {
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
    virtual void Remove();
    virtual void Start();
    virtual void v30();
    virtual void Resume();
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
    virtual void Activate();
};

struct Entry {
    int id;
    Unit *unit;
    char active;
    char resume;
    char remove;
    char started;
};

extern "C" {
extern char data_ov006_020d8de8[];
extern char data_020c9e88[];
int func_02008444(void);
void func_ov006_020d4e48(int object, int value);
void func_020521d4(char *object, int index, int a, int b);
void func_ov006_020d7240(char *object);
void func_0200fcc0(int object, int value);
void func_0201102c(int object);
int func_ov006_020d50e8(char *list, unsigned int index);
void func_ov006_020d50f4(struct Entry *entry, int source);
void func_02051594(Unit *unit);
void func_020322b4(int object);
void func_ov006_020d5008(char *object);
}

void func_ov006_020d5008(char *object)
{
    unsigned int i;
    int value;
    Unit *unit;
    struct Entry entry;

    func_ov006_020d4e48(func_02008444(), 0);
    if (object[0x27] == 0 && *(int *)(object + 0x20) == 0) {
        value = 6;
        func_020521d4(data_ov006_020d8de8, 0, value, value + 0xfa);
        func_020521d4(data_ov006_020d8de8, 1, value, value + 0xfa);
    }
    func_ov006_020d7240(object);
    func_0200fcc0(func_02008444(), 3);
    func_0201102c(func_02008444());
    for (i = 0; i < *(unsigned int *)(object + 0x664); i++) {
        func_ov006_020d50f4(&entry, func_ov006_020d50e8(object + 0x668, i));
        if (entry.remove != 0) {
            entry.unit->Remove();
            func_02051594(entry.unit);
        } else if (entry.active != 0) {
            unit = entry.unit;
            unit->Activate();
            if (entry.started == 0) {
                unit->Start();
            }
        } else if (entry.resume != 0) {
            entry.unit->Resume();
        }
    }
    *(unsigned short *)(data_020c9e88 + 0x26) = 0;
    *(unsigned short *)(data_020c9e88 + 0x24) = 0;
    func_020322b4(func_02008444());
}

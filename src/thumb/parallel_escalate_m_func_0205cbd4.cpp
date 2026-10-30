class Self {
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
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33(int);
};

class Part {
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
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33(int);
    virtual void slot34();
    virtual void slot35(signed char);
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42(unsigned short, int, int, int, int, int, int);
};

class Item {
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
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33(int);
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual int slot45();
};

extern "C" {
extern char data_020cfeb8;
extern char data_020cff0c;
extern char data_020ca0f0;
extern char data_020ca10c;
extern void *func_02056b2c(void *);
extern void *func_02056c8c(void *);
extern int func_0207dc50(int, int, int, void *, void *);
extern void func_020576d0(int, int);
extern unsigned char *func_02009900(void);
extern int func_0202fb48(unsigned char *, char *, int);
extern int func_02011898(Item *);
extern void func_02067650(Item *);

void func_0205cbd4(int *param_1)
{
    unsigned int i;

    ((Self *)param_1)->slot33(0x940);
    param_1[0x20] = func_0207dc50(2, 200, 8, (void *)func_02056b2c, (void *)func_02056c8c);
    i = 0;
    do {
        ((Part *)(param_1[0x20] + i * 200))->slot42((unsigned short)(i + 0xe), 0, 1, 1, 1, -1, 0);
        i = i + 1;
    } while (i < 2);
    param_1[0x13] = 0x29;
    *(short *)((char *)param_1 + 0x50) = 0x48;
    *(short *)((char *)param_1 + 0x52) = 1;
    param_1[0x12] = 0x1000;
    func_020576d0(param_1[0x20], 0x20000);
    ((Part *)param_1[0x20])->slot35((signed char)(*(short *)((char *)param_1 + 0x50) / 3));
    ((Part *)param_1[0x20])->slot33(0x940);
    func_020576d0(param_1[0x20] + 200, 0x10000);
    ((Part *)(param_1[0x20] + 200))->slot35(0x1f);
    ((Part *)(param_1[0x20] + 200))->slot33(0x8d0);
    param_1[0x15] = 0x1000;
    param_1[0x1c] = func_0202fb48(func_02009900(), &data_020cfeb8, -1);
    if (param_1[0x1c] != 0) {
        ((Item *)param_1[0x1c])->slot33(0x920);
        if (func_02011898((Item *)param_1[0x1c]) == 0) {
            func_02067650((Item *)param_1[0x1c]);
        }
    }
    param_1[0x1d] = func_0202fb48(func_02009900(), &data_020cff0c, -1);
    if (param_1[0x1d] != 0) {
        ((Item *)param_1[0x1d])->slot33(0x920);
        if (func_02011898((Item *)param_1[0x1d]) == 0) {
            func_02067650((Item *)param_1[0x1d]);
        }
    }
    param_1[0x1e] = func_0202fb48(func_02009900(), &data_020ca0f0, -1);
    if (param_1[0x1e] != 0) {
        ((Item *)param_1[0x1e])->slot33(0x920);
        if (((Item *)param_1[0x1e])->slot45() == 0) {
            ((Item *)param_1[0x1e])->slot42();
        }
    }
    param_1[0x1f] = func_0202fb48(func_02009900(), &data_020ca10c, -1);
    if (param_1[0x1f] != 0) {
        ((Item *)param_1[0x1f])->slot33(0x920);
        if (((Item *)param_1[0x1f])->slot45() == 0) {
            ((Item *)param_1[0x1f])->slot42();
        }
    }
}
}

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
    virtual void place(int x, int y, int z);
};

extern "C" {
extern char data_020cf438[];
extern char data_020cf498[];
extern char data_020cf454[];
Part *func_02054060(char *name);
Part *func_02054c68(char *name);
Part *func_02054038(char *name);
void *func_02009900(void);
void func_0202f7e8(void *manager, Part *part, int kind, int flag);

void func_02014d64(unsigned char *object)
{
    *(Part **)(object + 0x44) = func_02054060(data_020cf438);
    func_0202f7e8(func_02009900(), *(Part **)(object + 0x44), 7, 0);
    *(Part **)(object + 0x4c) = func_02054c68(data_020cf498);
    func_0202f7e8(func_02009900(), *(Part **)(object + 0x4c), 1, 0);
    *(Part **)(object + 0x40) = func_02054038(data_020cf454);
    func_0202f7e8(func_02009900(), *(Part **)(object + 0x40), 1, 0);
    (*(Part **)(object + 0x40))->place(0x80000, 0x100000, 0);
}
}

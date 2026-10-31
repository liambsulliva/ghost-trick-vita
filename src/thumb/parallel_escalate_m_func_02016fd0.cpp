class Object02016fd0 {
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
    virtual void slot33();
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
    virtual void slot45();
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual void slot53();
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59(int value);
};

extern "C" {
extern char data_020b9df4;
extern void func_02016fbc(int a, int b);
extern void func_02017714(int *a, int b);
extern void func_0201778c(int *a);
extern void func_0201c634(int a, unsigned int b, int c, int d, int e);
extern void func_0201c69c(int a, int b, int c, int d, int e);
extern void func_0201c6c4(int a, int b);
extern void func_0201cf2c(int a, int b);

void func_02016fd0(int *param_1, int param_2, unsigned int param_3)
{
    int uVar1;

    *(unsigned short *)(param_1 + 8) = 7;
    func_0201778c(param_1);
    param_1[6] = param_2;
    param_1[5] = param_2;
    func_02017714(param_1, 2);
    *(unsigned short *)((int)param_1 + 0x42) = 0;
    *(unsigned short *)(param_1 + 0x11) = 4;
    *(unsigned short *)((int)param_1 + 0x46) = 0x100;
    *(unsigned short *)(param_1 + 0x12) = 0x18;
    *(unsigned short *)(param_1 + 0x13) = *(unsigned short *)((int)param_1 + 0x46);
    *(unsigned short *)((int)param_1 + 0x4a) = *(unsigned short *)(param_1 + 0x13);
    uVar1 = (int)&data_020b9df4;
    func_0201c6c4(uVar1, param_1[4]);
    func_0201c634(uVar1, param_3, *(unsigned short *)((int)param_1 + 0x46), *(unsigned short *)(param_1 + 0x12), 2);
    func_0201cf2c(uVar1, param_1[0x18]);
    func_02016fbc(uVar1, 3);
    ((Object02016fd0 *)param_1)->slot59(param_1[5]);
    func_0201c69c(uVar1, param_1[5],
                  (short)(*(short *)((int)param_1 + 0x42) + *(short *)(param_1 + 0xf)),
                  *(short *)(param_1 + 0x11), *(unsigned short *)((int)param_1 + 0x2a) & 0xff);
}
}

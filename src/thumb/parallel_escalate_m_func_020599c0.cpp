typedef struct {
    unsigned char b0 : 1;
    unsigned char b1 : 7;
} Flags020599c0;

class Object020599c0 {
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
};

extern "C" {
int func_02010b54(int param_1);
unsigned int *func_02008444(void);
int func_02046490(char *object);
void func_02007c40(int param_1, int param_2);

void func_020599c0(int param_1)
{
    unsigned int off;
    int ptr;

    off = 0x141;
    if (func_02010b54(param_1) != 0) {
        if (((Flags020599c0 *)(param_1 + off))->b0) {
            ptr = func_02046490((char *)func_02008444());
            func_02007c40(param_1 + (off - 0x2d), ptr + *(int *)(param_1 + (off + 3)) * 2);
        } else {
            ((Object020599c0 *)param_1)->slot30();
        }
    }
    ((Flags020599c0 *)(param_1 + off))->b0 = 0;
}
}

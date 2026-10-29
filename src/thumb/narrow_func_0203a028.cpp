class Object0203a028_0 {
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
};

class Object0203a028_1 {
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
};

class Object0203a028_2 {
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
};

class Object0203a028_3 {
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
};

class Object0203a028_4 {
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
};

extern "C" {
typedef int code(...);
extern int func_02039564(...);
extern int func_0203a0a0(...);
extern int func_0203a0ac(...);
extern int func_0203a0b8(...);

void func_0203a028(unsigned int param_1,int *param_2,unsigned int param_3,int param_4)

{
    if (param_2 != (int *)0x0) {
        switch(param_3) {
        case 0:
            ((Object0203a028_0 *)(param_2))->slot15();
            return;
        case 1:
            func_0203a0a0((unsigned char *)param_2,param_4);
            return;
        case 2:
            func_0203a0ac((unsigned char *)param_2,param_4);
            return;
        case 3:
            ((Object0203a028_1 *)(param_2))->slot17();
            return;
        case 4:
            func_0203a0b8((unsigned char *)param_2);
            return;
        case 5:
            func_02039564((int)param_2,param_4);
            return;
        case 6:
            ((Object0203a028_2 *)(param_2))->slot14();
            return;
        case 7:
            ((Object0203a028_3 *)(param_2))->slot12();
            return;
        case 8:
            ((Object0203a028_4 *)(param_2))->slot13();
        }
    }
    return;
}
}

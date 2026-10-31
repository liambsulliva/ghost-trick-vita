class Part0204c698 {
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
    virtual void slot47(void *pos,int a,int b);
};
class Actor0204c698 {
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
    virtual void slot59();
    virtual void slot60();
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65();
};
class View0204c698 {
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
    virtual void *slot17();
};

extern "C" {
struct Vec0204c698 {
    int x;
    int y;
    int z;
};
extern View0204c698 *data_020ceaa4[];
extern void func_020471ec(struct Vec0204c698 *out,void *actor);
extern void func_020082d8(struct Vec0204c698 *dst,void *src);
extern void *func_02008444(void);
extern int func_0200f330(void *system);
extern void func_0204c784(Part0204c698 *part,int index,unsigned char flag,unsigned int value);

void func_0204c698(unsigned char *p)
{
    struct Vec0204c698 pos;
    struct Vec0204c698 view;
    struct Vec0204c698 local;
    View0204c698 *camera;
    unsigned int i;

    func_020471ec(&pos,p);
    if (*(short *)(p + 0xb6) < 0) {
        *(short *)(p + 0xb6) = 0x1a;
    }
    func_020082d8(&local,&pos);
    (*(Part0204c698 **)(p + 0xb8))->slot47(&local,*(int *)(p + 0xbc),*(short *)(p + 0xb6) << 12);
    if (func_0200f330(func_02008444()) == 0) {
        ((Actor0204c698 *)p)->slot65();
        camera = data_020ceaa4[1];
        if (camera != 0) {
            func_020082d8(&view,camera->slot17());
            func_0204c784(*(Part0204c698 **)(p + 0xb8),0,1,view.x + 0xdb000);
            func_0204c784(*(Part0204c698 **)(p + 0xb8),1,1,view.y + 0x50000);
            func_0204c784(*(Part0204c698 **)(p + 0xb8),2,1,view.x - 0xdc000);
            func_0204c784(*(Part0204c698 **)(p + 0xb8),3,1,view.y - 0x3a000);
        } else {
            i = 0;
            do {
                func_0204c784(*(Part0204c698 **)(p + 0xb8),i,0,0);
                i++;
            } while (i < 4);
        }
    }
}
}

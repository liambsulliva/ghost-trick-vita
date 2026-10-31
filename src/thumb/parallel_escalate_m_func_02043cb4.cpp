class Obj02043cb4 {
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
    virtual void slot44(void *part);
};

extern "C" {
struct Manager02043cb4 {
    void *(**table)(struct Manager02043cb4 *manager,unsigned int size,void *name);
};
extern char data_020a8170;
extern char data_0209b4e4;
extern void func_02043964(void *object);
extern void func_02042718(void *part);
extern void *func_0200883c(void);
extern void *func_0201a4e8(void *system);
extern void *func_0201e888(void *object,int a,int b,int c,int d);
extern unsigned int func_0200cb5c(void *object);
extern void *func_02007bd8(void);
extern struct Manager02043cb4 *func_02007b70(void *system);
extern void func_0207303c(void *src,void *dst,unsigned int size);
extern void *func_02008824(void *system);
extern void func_0201e7f8(void *loader,int index,unsigned int id,void *dst);
extern void func_020427b0(void *part,void *data,int a,int b,int c,int d,int e,int f,int g,int h,int i);

unsigned char *func_02043cb4(unsigned char *p)
{
    unsigned short buffer[16];
    void *src;
    unsigned int size;
    struct Manager02043cb4 *manager;
    int offset;
    unsigned int count;
    unsigned int i;

    func_02043964(p);
    *(void **)p = &data_020a8170;
    *(int *)(p + 0xd0) = 0;
    *(int *)(p + 0xd4) = 0;
    func_02042718(p + 0xd8);
    src = func_0201e888(func_0201a4e8(func_0200883c()),0,0x7c,1,0);
    size = func_0200cb5c(func_0201a4e8(func_0200883c()));
    manager = func_02007b70(func_02007bd8());
    *(void **)(p + 0xd4) = manager->table[2](manager,size,&data_0209b4e4);
    func_0207303c(src,*(void **)(p + 0xd4),size);
    func_0201e7f8(func_02008824(func_0200883c()),0,5,buffer);
    offset = *(int *)(*(int *)(p + 0xd4) + 0x14);
    count = *(unsigned int *)(*(int *)(p + 0xd4) + 0x18) >> 1;
    for (i = 0; i < count; i++) {
        *(unsigned short *)(i * 2 + (*(int *)(p + 0xd4) + offset)) = buffer[i];
    }
    func_020427b0(p + 0xd8,*(void **)(p + 0xd4),0,0,1,0,0xffff,1,-1,-1,0x100);
    ((Obj02043cb4 *)p)->slot44(p + 0xd8);
    return p;
}
}

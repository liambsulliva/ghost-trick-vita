class Object02042a68_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

extern "C" {
extern int func_02007b70(...);
extern unsigned char *func_02007bd8(...);
extern unsigned short *func_0200eb78(...);
extern int func_02022c8c(...);

struct Loader {
    int pad0[2];
    int sound;
    int pad1[2];
    int voice;
    char pad2[0x21];
    unsigned char playing;
    char pad3[0x16];
    int buffer;
};

void func_02042a68(struct Loader *loader)
{
    unsigned short *puVar1;
    unsigned char *puVar2;
    int *piVar3;
    int iVar4;

    iVar4 = loader->sound;
    if (iVar4 != -1) {
        puVar1 = func_0200eb78();
        func_02022c8c((int)puVar1, iVar4);
        loader->sound = -1;
        loader->playing = 0;
    }
    iVar4 = loader->voice;
    if (iVar4 != -1) {
        puVar1 = func_0200eb78();
        func_02022c8c((int)puVar1, iVar4);
        loader->voice = -1;
    }
    if (loader->buffer != 0) {
        puVar2 = func_02007bd8();
        iVar4 = func_02007b70(puVar2);
        if (iVar4 != 0) {
            puVar2 = func_02007bd8();
            piVar3 = (int *)func_02007b70(puVar2);
            ((Object02042a68_0 *)(piVar3))->slot4((int)(loader->buffer));
            loader->buffer = 0;
        }
    }
}
}

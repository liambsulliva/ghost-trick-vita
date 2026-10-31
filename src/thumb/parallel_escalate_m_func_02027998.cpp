struct Vector {
    int x;
    int y;
    int z;
};
class Effect {
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
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
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
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8();
    virtual void SetPosition(struct Vector *position, unsigned int a, unsigned int b);
    virtual void SetC0(int value);
    virtual void SetC4(int value);
};
extern "C" {
extern int data_020cf8d0[];
extern Effect *func_02059ed0(int);
extern unsigned int func_020265c4(unsigned char *);
extern void func_02008398(struct Vector *, unsigned int, unsigned int, unsigned int);
extern unsigned char *func_02027c28(unsigned int);
extern void func_02027a4c(Effect *, int);
extern void func_020082d8(struct Vector *, struct Vector *);
extern void func_02027a54(Effect *, unsigned int);
extern void func_02027a5c(Effect *, int);
extern void func_02027a60(Effect *, int);
extern unsigned char *func_02009900(void);
extern void func_0202f7e8(int, Effect *, int, int);

void func_02027998(int param_1, int param_2)
{
    Effect *effect;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned char *puVar4;
    struct Vector auStack_24;
    struct Vector auStack_30;

    effect = func_02059ed0((int)data_020cf8d0);
    uVar2 = func_020265c4((unsigned char *)(param_1 + 4));
    uVar3 = func_020265c4((unsigned char *)(param_1 + 8));
    func_02008398(&auStack_24, uVar2, uVar3, 0);
    puVar4 = func_02027c28((unsigned int)*(unsigned short *)(param_1 + 2));
    func_02027a4c(effect, (int)puVar4);
    func_020082d8(&auStack_30, &auStack_24);
    uVar2 = func_020265c4((unsigned char *)(param_1 + 0xc));
    uVar3 = func_020265c4((unsigned char *)(param_1 + 0x10));
    effect->SetPosition(&auStack_30, uVar2, uVar3);
    func_02027a54(effect, *(unsigned short *)(param_1 + 0x14) & 0xff);
    func_02027a5c(effect, param_2);
    func_02027a60(effect, (int)*(short *)(param_1 + 0x16));
    effect->SetC0((int)*(short *)(param_1 + 0x18));
    effect->SetC4((int)*(short *)(param_1 + 0x1a));
    puVar4 = func_02009900();
    func_0202f7e8((int)puVar4, effect, 5, 0);
}
}

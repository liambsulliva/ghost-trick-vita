struct Vector {
    int x;
    int y;
    int z;
};
class Shot {
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
    virtual void SetB8(int value);
};
extern "C" {
extern int data_020cf8ec[];
extern Shot *func_0205a19c(int);
extern unsigned int func_020265c4(unsigned char *);
extern void func_02008398(struct Vector *, unsigned int, unsigned int, unsigned int);
extern unsigned char *func_02027c28(unsigned int);
extern void func_02027888(Shot *, int);
extern void func_0205a438(Shot *, struct Vector *, struct Vector *, unsigned int, unsigned int, unsigned int, unsigned char);
extern unsigned char *func_02009900(void);
extern void func_0202f7e8(int, Shot *, int, int);

void func_020277d8(int param_1)
{
    Shot *shot;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar5;
    unsigned char *puVar4;
    struct Vector start;
    struct Vector end;

    shot = func_0205a19c((int)data_020cf8ec);
    uVar2 = func_020265c4((unsigned char *)(param_1 + 4));
    uVar3 = func_020265c4((unsigned char *)(param_1 + 8));
    func_02008398(&start, uVar2, uVar3, 0);
    uVar2 = func_020265c4((unsigned char *)(param_1 + 0xc));
    uVar3 = func_020265c4((unsigned char *)(param_1 + 0x10));
    func_02008398(&end, uVar2, uVar3, 0);
    puVar4 = func_02027c28((unsigned int)*(unsigned short *)(param_1 + 2));
    func_02027888(shot, (int)puVar4);
    uVar2 = func_020265c4((unsigned char *)(param_1 + 0x14));
    uVar3 = func_020265c4((unsigned char *)(param_1 + 0x18));
    uVar5 = func_020265c4((unsigned char *)(param_1 + 0x1c));
    func_0205a438(shot, &start, &end, uVar2, uVar3, uVar5, *(unsigned short *)(param_1 + 0x20));
    shot->SetB8((int)*(short *)(param_1 + 0x22));
    puVar4 = func_02009900();
    func_0202f7e8((int)puVar4, shot, 5, 0);
}
}

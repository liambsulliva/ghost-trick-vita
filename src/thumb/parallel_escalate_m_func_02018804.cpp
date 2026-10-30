class Object02018804_0 {
public:
    virtual void slot0(int);
    virtual void slot1(int);
    virtual void slot2(int);
    virtual void slot3(int);
    virtual void slot4(int);
    virtual void slot5(int);
    virtual void slot6(int);
    virtual void slot7(int);
    virtual void slot8(int);
    virtual void slot9(int);
    virtual void slot10(int);
    virtual void slot11(int);
    virtual void slot12(int);
    virtual void slot13(int);
    virtual void slot14(int);
    virtual void slot15(int);
    virtual void slot16(int);
    virtual void slot17(int);
    virtual void slot18(int);
    virtual void slot19(int);
    virtual void slot20(int);
    virtual void slot21(int);
    virtual void slot22(int);
    virtual void slot23(int);
    virtual void slot24(int);
    virtual void slot25(int);
    virtual void slot26(int);
    virtual void slot27(int);
    virtual void slot28(int);
    virtual void slot29(int);
    virtual void slot30(int);
    virtual void slot31(int);
    virtual void slot32(int);
    virtual void slot33(int);
    virtual void slot34(int);
    virtual void slot35(int);
    virtual void slot36(int);
    virtual void slot37(int);
    virtual void slot38(int);
    virtual void slot39(int);
    virtual void slot40(int);
    virtual void slot41(int);
    virtual void slot42(int);
    virtual void slot43(int);
    virtual void slot44(int);
    virtual void slot45(int);
    virtual void slot46(int);
    virtual void slot47(int);
    virtual void slot48(int);
    virtual void slot49(int);
    virtual void slot50(int);
    virtual void slot51(int);
    virtual void slot52(int);
    virtual void slot53(int);
    virtual void slot54(int);
    virtual void slot55(int);
    virtual void slot56(int);
    virtual void slot57(int);
    virtual void slot58(int);
    virtual void slot59(int);
    virtual void slot60(int);
    virtual void slot61(int);
    virtual void slot62(int);
    virtual void slot63(int);
    virtual void slot64(int);
    virtual void slot65(int);
    virtual void slot66(int);
    virtual void slot67(int);
    virtual void slot68(int);
    virtual void slot69(int);
    virtual void slot70(int);
    virtual void slot71(int);
    virtual void slot72(int);
    virtual void slot73(int);
    virtual void slot74(int);
    virtual void slot75(int);
    virtual void slot76(int);
    virtual void slot77(int);
    virtual void slot78(int);
    virtual void slot79(int);
    virtual void slot80(int);
    virtual void slot81(int);
    virtual void slot82(int);
    virtual void slot83(int);
    virtual void slot84(int);
    virtual void slot85(int);
    virtual void slot86(int);
    virtual void slot87(int);
    virtual void slot88(int);
    virtual void slot89(int);
    virtual void slot90(int);
    virtual void slot91(int);
};

extern "C" {
extern void func_020164a8(int *param_1, int param_2);
extern void func_0201bfec(int param_1);
extern void func_0201c01c(int param_1);

void func_02018804(int *param_1, int param_2)
{
    int iVar1;

    func_020164a8(param_1, param_2);
    iVar1 = param_1[0x20];
    if (iVar1 != 0) {
        if (param_2 != 0) {
            func_0201c01c(iVar1);
        }
        else {
            func_0201bfec(iVar1);
        }
    }
    ((Object02018804_0 *)param_1)->slot91(param_2);
    *(unsigned char *)((int)param_1 + 0x58) = 1;
}
}

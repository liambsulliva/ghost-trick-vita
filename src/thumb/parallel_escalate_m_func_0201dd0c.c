struct Panel {
    unsigned char pad000[0xaa8];
    unsigned char aa8_b0 : 1;
    unsigned char aa8_b1 : 1;
    unsigned char aa8_rest : 6;
    unsigned char aa9_low : 7;
    unsigned char aa9_b7 : 1;
};
extern void func_ov004_020d5ba8(struct Panel *panel);

void func_0201dd0c(struct Panel *panel)
{
    if (panel->aa8_b1 != 0 && panel->aa9_b7 == 0) {
        panel->aa9_b7 = 1;
        func_ov004_020d5ba8(panel);
    }
}

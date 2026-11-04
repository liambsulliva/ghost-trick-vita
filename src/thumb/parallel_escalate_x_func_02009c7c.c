extern char data_020a2bbc;
extern char data_020c9e74;
extern int func_02009dd0();
extern int func_02009ddc();
extern void func_02009fe8();
extern void func_0200a088();
extern void func_0206f588();

void func_02009c7c(unsigned char *param_1)
{
    unsigned int cy;
    unsigned int cx;
    int wv;
    int hv;
    unsigned int sx;
    unsigned int sy;
    int y;
    int x;

    if (func_02009dd0(param_1) != 0) {
        return;
    }
    if (func_02009ddc(param_1) == 0) {
        return;
    }
    x = (short)(*(short *)(param_1 + 0xaa30) + *(short *)(param_1 + 0xaa38));
    y = (short)(*(short *)(param_1 + 0xaa32) + *(short *)(param_1 + 0xaa3a));
    if (x != *(short *)(param_1 + 0xaa34) || y != *(short *)(param_1 + 0xaa36)) {
        param_1[0xaa40] = 1;
    }
    *(short *)(param_1 + 0xaa2e) = 0;
    if (param_1[0xaa40] == 0) {
        return;
    }
    cx = (unsigned char)((x + 0xf) >> 4);
    cy = (unsigned char)((y + 0xf) >> 4);
    if (cx != param_1[0xaa3c] || cy != param_1[0xaa3d]) {
        sx = cx - 1;
        if (cx == 0) {
            sx = 0;
        }
        sy = cy != 0 ? cy - 1 : 0;
        if (sx + 0x12 > (unsigned int)*(unsigned short *)(param_1 + 0xaa28) >> 4) {
            wv = ((unsigned int)*(unsigned short *)(param_1 + 0xaa28) >> 4) - sx;
        } else if (sx == 0) {
            wv = 0x11;
        } else {
            wv = 0x12;
        }
        if (sy + 0xe > (unsigned int)*(unsigned short *)(param_1 + 0xaa2a) >> 4) {
            hv = ((unsigned int)*(unsigned short *)(param_1 + 0xaa2a) >> 4) - sy;
        } else if (sy == 0) {
            hv = 0xd;
        } else {
            hv = 0xe;
        }
        func_02009fe8(param_1, sx, sy, wv, hv);
        func_0200a088(param_1, sx, sy, wv, hv);
        param_1[0xaa3c] = cx;
        param_1[0xaa3d] = cy;
        param_1[0xaa40] = 0;
    }
    func_0206f588(&data_020c9e74, &data_020a2bbc, 0x80, 0x60, x, y);
    *(short *)(param_1 + 0xaa34) = x;
    *(short *)(param_1 + 0xaa36) = y;
}

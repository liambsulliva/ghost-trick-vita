typedef struct Oam3f850 {
    unsigned int attr01;
    unsigned short attr2;
    unsigned short affine;
} Oam3f850;

void func_0203f850(Oam3f850 *oam, int x, int y, int priority, int mode, int mosaic, int effect, int shape, int color, int charName, int cParam, int rsParam)
{
    if (effect == 0x100 || effect == 0x300) {
        if (mode == 3) {
            oam->attr01 = (rsParam << 25) | (y & 0xff) | (mode << 10) | (mosaic << 12) | shape | ((x & 0x1ff) << 16) | effect;
        } else {
            oam->attr01 = (rsParam << 25) | (color << 13) | (y & 0xff) | (mode << 10) | (mosaic << 12) | shape | ((x & 0x1ff) << 16) | effect;
        }
    } else {
        if (mode == 3) {
            oam->attr01 = (y & 0xff) | (mode << 10) | (mosaic << 12) | shape | ((x & 0x1ff) << 16) | effect;
        } else {
            oam->attr01 = (color << 13) | (y & 0xff) | (mode << 10) | (mosaic << 12) | shape | ((x & 0x1ff) << 16) | effect;
        }
    }
    oam->attr2 = charName | (priority << 10) | (cParam << 12);
}

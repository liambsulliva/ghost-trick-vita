struct Effect {
    unsigned char before[0x130];
    unsigned char low : 5;
    unsigned char mode : 3;
};

void func_020105b0(struct Effect *effect,unsigned char mode)
{
    effect->mode = mode;
    if (effect->mode != 1) {
        effect->mode = 0;
    }
}

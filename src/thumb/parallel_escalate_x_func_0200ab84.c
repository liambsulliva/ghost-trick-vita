struct Wave {
    unsigned char before[0x10];
    unsigned char timer;
    unsigned char middle[0x17];
    int base;
    int frame;
    int slot;
    int angle;
};
struct Matrix {
    int m[4];
};
extern struct Matrix data_020a2bbc;
extern char data_020c9e74;
extern char data_020a2bb8;
extern int *func_02009c1c(void);
extern void func_0202f1bc(int *table,int a,int b,int c,int d);
extern int func_0200ac04(int value);
extern int func_0200ac14(int value);
extern void func_0206f588(void *reg,struct Matrix *matrix,int a,int b,int c,int d);

void func_0200ab84(struct Wave *wave)
{
    int sin;
    int cos;

    wave->timer--;
    if (wave->timer == 0) {
        wave->timer = 16;
        wave->frame = (wave->frame + 1) & 0xf;
        func_0202f1bc(func_02009c1c(),wave->base + wave->frame * 32,(wave->slot << 9) + 0x6000,0x20,16);
    }
    wave->angle = (unsigned short)(wave->angle + 0x20);
    sin = func_0200ac04(wave->angle);
    cos = func_0200ac14(wave->angle);
    data_020a2bbc.m[0] = cos;
    data_020a2bbc.m[1] = sin;
    data_020a2bbc.m[2] = -sin;
    data_020a2bbc.m[3] = cos;
    func_0206f588(&data_020c9e74,(struct Matrix *)(&data_020a2bb8 + 4),0x80,0x80,0,0x20);
}

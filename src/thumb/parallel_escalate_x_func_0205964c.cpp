class Actor {
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
};

struct Flags140 {
    unsigned char index : 4;
    unsigned char reset : 1;
    unsigned char loop : 1;
    unsigned char pending : 1;
    unsigned char shift : 1;
};

struct Flags142 {
    unsigned char low : 7;
    unsigned char restart : 1;
};

struct FlagsDE {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char keep : 1;
    unsigned char alt : 1;
    unsigned char b5 : 1;
    unsigned char mode : 2;
};

extern "C" {
extern int func_02059810(void *anim);
extern int func_02059814(Actor *self);
extern void func_02007c54(void *anim);
extern void func_02007c14(void *anim, int data, bool loop, int start);
extern void *func_02008444(void);
extern int func_0200f320(void *system);
extern void func_02059128(Actor *self);
extern void func_0200bb8c(void *sprite, unsigned int value);
extern void func_020591a4(Actor *self, int index, unsigned int mode);
extern void func_0205958c(int *dst, void *self, unsigned int index);

void func_0205964c(Actor *self)
{
    char *base;
    int pos[3];

    base = (char *)self;
    if (*(int *)(base + 0x148) != 0) {
        if (((Flags140 *)(base + 0x140))->pending && func_02059810(base + 0x114) != 0) {
            if (func_02059814(self) != 0) {
                func_02007c54(base + 0x114);
            }
            return;
        }
        if (((Flags142 *)(base + 0x142))->restart) {
            ((Flags142 *)(base + 0x142))->restart = 0;
            func_02007c14(base + 0x114, *(int *)(base + 0x148), ((Flags140 *)(base + 0x140))->loop, *(unsigned char *)(base + 0x143));
        } else {
            func_02007c14(base + 0x114, *(int *)(base + 0x148), ((Flags140 *)(base + 0x140))->loop, -1);
        }
        *(int *)(base + 0x148) = 0;
        ((Flags140 *)(base + 0x140))->loop = 0;
        ((Flags140 *)(base + 0x140))->pending = 0;
        if (!((FlagsDE *)(base + 0xde))->alt) {
            self->v28();
        } else {
            self->v2c();
        }
        if (func_0200f320(func_02008444()) != 0) {
            func_02059128(self);
        }
    } else if (!((FlagsDE *)(base + 0xde))->keep && func_02059814(self) != 0) {
        func_02007c54(base + 0x114);
    }
    if (*(unsigned short *)(base + 0xc0) != 0) {
        func_0200bb8c(base + 0x28, *(unsigned short *)(base + 0xc0));
        *(unsigned short *)(base + 0xc0) = 0;
    }
    if (*(signed char *)(base + 0xbf) != -1) {
        func_020591a4(self, *(signed char *)(base + 0xbf), ((FlagsDE *)(base + 0xde))->mode);
        *(signed char *)(base + 0xbf) = -1;
    }
    if (((Flags140 *)(base + 0x140))->reset) {
        ((Flags140 *)(base + 0x140))->reset = 0;
        func_0205958c(pos, self, ((Flags140 *)(base + 0x140))->index);
        *(int *)(base + 0xb0) = (*(short *)(base + 0x14c) << 12) - pos[0];
        *(int *)(base + 0xb4) = (*(short *)(base + 0x14e) << 12) - pos[1];
    }
    if (((Flags140 *)(base + 0x140))->shift) {
        ((Flags140 *)(base + 0x140))->shift = 0;
        *(int *)(base + 0xb0) += *(short *)(base + 0x150) << 12;
        *(int *)(base + 0xb4) += *(short *)(base + 0x152) << 12;
    }
}
}

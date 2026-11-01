struct Vec44a0c {
    int x;
    int y;
    int z;
};

struct Target44a0c {
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual int s16();
    virtual int s17();
};

struct Obj44a0c {
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12(Vec44a0c *a);
    char pad04[0x24];
    Vec44a0c pos;
    char pad34[0x91];
    unsigned char follow;
    unsigned char fixed;
    char padc7;
    int baseX;
    int baseY;
    char padd0[0x14];
    int offX;
    int offY;
    char padec[8];
    Vec44a0c screen;
};

extern "C" {
void *func_02008444(void);
int func_0200f5c8(void *a);
int func_0200f5e8(void *a);
void func_020082d8(Vec44a0c *dst, Vec44a0c *src);
Vec44a0c *func_020083a0(Target44a0c *a);
void func_020083b0(Vec44a0c *dst, Vec44a0c *src);
Target44a0c *func_02029640(Obj44a0c *a);
}

extern "C" void func_02044a0c(Obj44a0c *p)
{
    Vec44a0c world;
    Vec44a0c view;
    Target44a0c *target;
    int moved;
    int t;

    moved = 0;
    func_020082d8(&world, &p->pos);
    if (p->fixed != 0) {
        world.x = p->baseX;
        world.y = p->baseY;
    } else if (p->follow != 0) {
            moved = 1;
            world.x = p->baseX - func_0200f5c8(func_02008444());
            world.y = p->baseY - func_0200f5e8(func_02008444());
    } else if ((target = func_02029640(p)) != 0) {
            moved = 1;
            func_020083b0(&world, func_020083a0(target));
            t = target->s16();
            world.x += p->baseX + (t - func_0200f5c8(func_02008444()));
            t = target->s17();
            world.y += p->baseY + (t - func_0200f5e8(func_02008444()));
    } else {
        world.x = p->baseX;
        world.y = p->baseY;
    }
    world.x += p->offX;
    world.y += p->offY;
    func_020082d8(&view, &world);
    view.x = view.x - 0x80000;
    view.y = 0x60000 - view.y;
    func_020083b0(&p->screen, &view);
    if (moved) {
        p->screen.x += func_0200f5c8(func_02008444());
        p->screen.y -= func_0200f5e8(func_02008444());
    }
    p->s12(&view);
}

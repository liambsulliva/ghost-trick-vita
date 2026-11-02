struct Dust {
    unsigned char fade;
    unsigned char active;
    unsigned char pad2[0x2];
    int x;
    int y;
    int vx;
    int vy;
    int z;
    int vz;
    int alpha;
};

class Owner {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8(int value);
    unsigned char pad4[0x24];
    int x;
    int y;
    unsigned char pad30[0x88];
    Dust *dust;
    unsigned int count;
    unsigned char stop;
    unsigned char always;
};

extern "C" {
extern void *func_02008444(void);
extern int func_0200f330(void *game);
extern void *func_02008eb0(void);
extern unsigned int func_01ff80b8(void *random);
extern int func_0206f28c(int a, int b);
void func_02048924(Owner *p);
}

void func_02048924(Owner *p)
{
    int spawned;
    unsigned int i;
    int done;

    if (p->dust == 0) {
        p->s8(0);
        return;
    }
    if (func_0200f330(func_02008444()) != 0 && p->always == 0) {
        return;
    }
    spawned = 0;
    for (i = 0; i < 0x20; i++) {
        if (p->dust[i].active == 0) {
            if (spawned == 0 && p->count == 0 && p->stop == 0) {
                p->dust[i].vx = -(func_01ff80b8(func_02008eb0()) & 0x7ff);
                p->dust[i].vy = -((func_01ff80b8(func_02008eb0()) & 0x7d0) + 0xbb8);
                p->dust[i].x = p->x;
                p->dust[i].y = p->y;
                p->dust[i].vz = (func_01ff80b8(func_02008eb0()) & 0x3f) + 0x3f;
                spawned = 1;
                p->dust[i].active = 1;
                p->dust[i].alpha = 0;
                p->dust[i].fade = 1;
                p->dust[i].z = 0x19a;
            }
        } else {
            p->dust[i].x += p->dust[i].vx;
            p->dust[i].y += p->dust[i].vy;
            p->dust[i].z += p->dust[i].vz;
            p->dust[i].vx = func_0206f28c(p->dust[i].vx, 0x1000);
            p->dust[i].vy = func_0206f28c(p->dust[i].vy, 0xfd7);
            p->dust[i].vz = func_0206f28c(p->dust[i].vz, 0xe14);
            if (p->dust[i].vz < 0x10) {
                p->dust[i].vz = 0;
            }
            if (p->dust[i].fade != 0) {
                p->dust[i].alpha += 0x800;
                if (p->dust[i].alpha >= 0x10000) {
                    p->dust[i].fade = 0;
                }
            } else {
                p->dust[i].alpha -= 0x100;
            }
            if (p->dust[i].alpha < 0) {
                p->dust[i].active = 0;
            }
        }
    }
    p->count++;
    if (p->count > 8) {
        p->count = 0;
    }
    if (p->stop == 0) {
        return;
    }
    done = 1;
    for (i = 0; i < 0x20; i++) {
        if (p->dust[i].active != 0) {
            done = 0;
            break;
        }
    }
    if (done == 0) {
        return;
    }
    p->s8(0);
}

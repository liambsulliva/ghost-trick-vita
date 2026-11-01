struct Tilt {
    unsigned char pad0[0xb0];
    int target;
    unsigned char padb4[0x1c];
    int current;
    unsigned char padd4[0xa];
    signed char phase;
    signed char side;
    unsigned char pade0;
    unsigned char last;
};

extern void func_02039294(struct Tilt *p, int v);
extern void func_020392b0(struct Tilt *p, int v);

void func_02039564(struct Tilt *p, int d)
{
    int amount;

    if (p->side == 0) {
        p->side = d;
    }
    amount = d;
    switch (p->phase) {
    case -1:
        if (p->side < 0) {
            func_02039294(p, -(d * 5) << 12);
        } else if (p->side > 0) {
            func_02039294(p, -(d * 4) << 12);
        }
        func_020392b0(p, d * 0x7000);
        break;
    case 0:
        if (p->side < 0) {
            func_02039294(p, -(d * 4) << 12);
        } else if (p->side > 0) {
            func_02039294(p, -(d * 3) << 12);
        }
        func_020392b0(p, d * 0x7000);
        break;
    case 1:
        if (p->side < 0) {
            func_02039294(p, -(d * 3) << 12);
            func_020392b0(p, d * 0x7000);
        } else if (p->side > 0) {
            func_02039294(p, -(d * 2) << 12);
            func_020392b0(p, d << 15);
        }
        break;
    case 2:
        if (p->target == p->current) {
            if (d < 0) {
                amount = -d;
            }
            func_02039294(p, amount << 13);
        } else if (p->side < 0) {
            func_02039294(p, -(d * 2) << 12);
        } else if (p->side > 0) {
            func_02039294(p, d << 13);
        }
        func_020392b0(p, d << 15);
        break;
    case 3:
        if (p->side < 0) {
            func_02039294(p, d << 13);
            func_020392b0(p, d << 15);
        } else if (p->side > 0) {
            func_02039294(p, (d * 3) << 12);
            func_020392b0(p, d * 0x7000);
        }
        break;
    case 4:
        if (p->side < 0) {
            func_02039294(p, (d * 3) << 12);
        } else if (p->side > 0) {
            func_02039294(p, d << 14);
        }
        func_020392b0(p, d * 0x7000);
        break;
    case 5:
        if (p->side < 0) {
            func_02039294(p, d << 14);
        } else if (p->side > 0) {
            func_02039294(p, (d * 5) << 12);
        }
        func_020392b0(p, d * 0x7000);
        break;
    }
    p->last = d;
}

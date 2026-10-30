typedef struct Actor {
    char pad[0x8a];
    unsigned char active;
    char pad2[0xd4 - 0x8b];
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char rest : 5;
} Actor;

extern void *func_02008444(void);
extern int func_0202c394(void *);
extern int func_0200f330(void *);
extern int func_0203435c(void *);
extern void func_02066108(Actor *);

void func_0206705c(Actor *actor)
{
    if (actor->b2) {
        if (func_0202c394(func_02008444()) == 0) {
            return;
        }
    } else if (!actor->b1) {
        if (func_0202c394(func_02008444()) != 0) {
            return;
        }
    }
    if (actor->active != 0) {
        if (func_0200f330(func_02008444()) != 0) {
            return;
        }
    }
    if (func_0203435c(func_02008444()) != 0) {
        return;
    }
    func_02066108(actor);
}

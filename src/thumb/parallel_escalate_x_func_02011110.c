typedef struct Room11110 {
    char pad0[8];
    unsigned int low : 11;
    unsigned int busy : 1;
    unsigned int mid : 4;
    unsigned int mode : 8;
    unsigned int high : 8;
    char padc[0xe4c];
    unsigned int request;
} Room11110;

extern char data_020cd92c[];
extern char data_020ce444[];
extern char data_020cdcf0[];
extern char data_020cd9d8[];
extern char data_020ce0f0[];
extern void *func_02011264(void);
extern void *func_0201123c(void *a, int b);
extern int func_02022f74(void *a, char *name);
extern int func_02022fc0(void *a, char *name);
extern void func_02022f2c(void *a, char *name, int b);
extern void func_02011080(Room11110 *p, int a);
extern void func_0201134c(Room11110 *p);
extern void func_02011598(Room11110 *p);
extern void func_0201135c(Room11110 *p);
extern int func_02011244(void);

void func_02011110(Room11110 *p)
{
    switch (p->request) {
    case 2:
        p->request = 0;
        if (p->mode == 2) {
            return;
        }
        if (func_02022f74(func_0201123c(func_02011264(), 0), data_020cd92c) != 0) {
            return;
        }
        func_02011080(p, 2);
        func_02022f2c(func_0201123c(func_02011264(), 0), data_020cd92c, 0);
        if (func_02022fc0(func_0201123c(func_02011264(), 1), data_020ce444) != 0) {
            func_0201134c(p);
        }
        func_02011598(p);
        break;
    case 3:
        p->busy = 0;
        func_02011080(p, 3);
        if (func_02022fc0(func_0201123c(func_02011264(), 0), data_020cdcf0) == 0 && func_02022fc0(func_0201123c(func_02011264(), 0), data_020cd9d8) == 0) {
            func_02022f2c(func_0201123c(func_02011264(), 0), data_020cdcf0, 0);
        }
        func_0201135c(p);
        break;
    case 0x18:
        if (func_02011244() != 0) {
            break;
        }
        func_02011080(p, 3);
        func_02022f2c(func_0201123c(func_02011264(), 0), data_020cd9d8, 0);
        break;
    case 0x19:
        func_02022f2c(func_0201123c(func_02011264(), 0), data_020ce0f0, 0);
        break;
    }
    p->request = 0;
}

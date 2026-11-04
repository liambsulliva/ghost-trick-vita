#pragma opt_common_subs off
#pragma opt_propagation off
typedef struct CtrdgHeader02079f34 {
    char pad0[0xac];
    unsigned int gameCode;
    unsigned short makerCode;
    char padb2[3];
    unsigned char unit[3];
    char padb8[6];
    unsigned short check;
} CtrdgHeader02079f34;

typedef struct CtrdgLock02079f34 {
    unsigned short state;
    unsigned short lockId;
} CtrdgLock02079f34;

typedef struct CtrdgInfo02079f34 {
    unsigned short check;
    unsigned char unit[3];
    char pad5;
    unsigned short makerCode;
    unsigned int gameCode;
} CtrdgInfo02079f34;

extern CtrdgLock02079f34 data_020d2f00;
extern CtrdgHeader02079f34 data_020d2f20;
extern char data_020d2f24[];

extern unsigned int func_02073594(unsigned int mask);
extern void func_02079d24(unsigned int lockId, int *lock);
extern void func_02079ca8(int *cycle);
extern void func_02074844(unsigned int start, unsigned int size);
extern void func_02072a18(unsigned int dmaNo, unsigned int src, unsigned int dest, unsigned int size, int flag);
extern void func_02079cf0(int *cycle);
extern void func_02079d80(unsigned int lockId, int *lock);
extern int func_02079ba4(void);
extern void func_02072ed8(const void *srcp, void *destp, unsigned int size);
extern void func_02074810(void);
extern void func_02079da4(int data);
extern void WaitByLoop(int count);

void func_02079f34(void)
{
    static int data_020d2f04;
    unsigned int mask;
    unsigned short prepIme;
    int lock[2];
    int cycle[2];
    int phi;
    unsigned short post;
    int i;
    CtrdgHeader02079f34 *hdr;
    CtrdgInfo02079f34 *info;

    if (data_020d2f04 != 0) {
        return;
    }
    post = *(volatile unsigned short *)0x04000300;
    data_020d2f04 = 1;
    if (!(post & 1)) {
        return;
    }

    mask = func_02073594(0x40000);
    prepIme = *(volatile unsigned short *)0x04000208;
    *(volatile unsigned short *)0x04000208 = 1;

    func_02079d24(data_020d2f00.lockId, lock);
    phi = (*(volatile unsigned short *)0x04000204 & 0x8000) >> 15;
    func_02079ca8(cycle);
    *(volatile unsigned short *)0x04000204 = *(volatile unsigned short *)0x04000204 & ~0x8000;
    func_02074844((unsigned int)&data_020d2f20 + 0x80, 0x40);
    func_02072a18(1, 0x08000080, (unsigned int)&data_020d2f20 + 0x80, 0x40, 1);
    *(volatile unsigned short *)0x04000204 = (*(volatile unsigned short *)0x04000204 & ~0x8000) | (phi << 15);
    func_02079cf0(cycle);
    func_02079d80(data_020d2f00.lockId, lock);

    if (*(unsigned char *)0x02ffff9b != 0 || *(unsigned char *)0x02ffff9a == 0) {
        hdr = &data_020d2f20;
        info = (CtrdgInfo02079f34 *)0x02fffc30;
        info->check = hdr->check;
        for (i = 0; i < 3; i++) {
            info->unit[i] = hdr->unit[i];
        }
        info->makerCode = hdr->makerCode;
        info->gameCode = hdr->gameCode;
        *(unsigned char *)0x02ffff9b = func_02079ba4() ? 1 : 0;
        *(unsigned char *)0x02ffff9a = 1;
    }

    func_02072ed8((const void *)0xffff0020, data_020d2f24, 156);
    func_02074810();
    func_02079da4(((((unsigned int)&data_020d2f20 - 0x02000000) >> 5) << 6) | 1);
    while (data_020d2f00.state != 1) {
        WaitByLoop(1);
    }

    *(volatile unsigned short *)0x04000208;
    *(volatile unsigned short *)0x04000208 = prepIme;
    func_02073594(mask);
}

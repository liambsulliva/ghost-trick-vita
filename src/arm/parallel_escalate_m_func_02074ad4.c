struct ArenaStatics {
    int initialized;
    int mainExEnabled;
};

extern struct ArenaStatics data_020d1180;
extern char data_00000000[];
extern char data_00000800[];
extern char data_027e0000[];
extern unsigned int func_02074500(void);

void *func_02074ad4(int id)
{
    switch (id) {
    case 0:
        return (void *)0x023e0000;
    case 2:
        if (!data_020d1180.mainExEnabled) {
            return 0;
        }
        if ((func_02074500() & 0xf) == 1) {
            return 0;
        }
        return (void *)0x02700000;
    case 3:
        return (void *)0x02000000;
    case 4:
        {
            unsigned int irqStackLo = (unsigned int)data_027e0000 + 0x3f80 - (int)data_00000800;
            unsigned int sysStackLo;

            if ((int)data_00000000 == 0) {
                sysStackLo = (unsigned int)data_027e0000;
                if (sysStackLo < 0x027e0080) {
                    sysStackLo = 0x027e0080;
                }
            } else if ((int)data_00000000 < 0) {
                sysStackLo = 0x027e0080 - (int)data_00000000;
            } else {
                sysStackLo = irqStackLo - (int)data_00000000;
            }
            return (void *)sysStackLo;
        }
    case 5:
        return (void *)0x02fff680;
    case 6:
        return (void *)0x037f8000;
    }
    return 0;
}

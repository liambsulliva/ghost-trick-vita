struct ArenaStatics {
    int initialized;
    int mainExEnabled;
};

extern struct ArenaStatics data_020d1180;
extern unsigned int func_02074500(void);

void *func_02074bb4(int id)
{
    switch (id) {
    case 0:
        return (void *)0x020e4860;
    case 2:
        if (!data_020d1180.mainExEnabled) {
            return 0;
        }
        if ((func_02074500() & 0xf) == 1) {
            return 0;
        }
        return (void *)0x023e0000;
    case 3:
        return (void *)0x01ff8620;
    case 4:
        return (void *)0x027e0080;
    case 5:
        return (void *)0x02fff000;
    case 6:
        return (void *)0x037f8000;
    }
    return 0;
}

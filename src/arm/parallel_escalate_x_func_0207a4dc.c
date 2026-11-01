typedef struct Entry7a4dc {
    int f0;
    unsigned int low : 7;
    unsigned int state : 3;
    unsigned int high : 22;
    char pad[0x44];
} Entry7a4dc;

extern Entry7a4dc data_020b17ac[3];
extern int func_0207a5fc(Entry7a4dc *entry);

int func_0207a4dc(void)
{
    int result = 0;
    int i = 1;
    Entry7a4dc *entry = &data_020b17ac[0];
    do {
        if (entry->state != 0) {
            if (func_0207a5fc(entry) != 0) {
                result = -1;
            }
        }
        entry = (i < 3) ? &data_020b17ac[i++] : 0;
    } while (entry != 0);
    return result;
}

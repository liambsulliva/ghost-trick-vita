struct Source {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3(unsigned char *buf);
};

struct Header {
    int word0;
    unsigned int count : 15;
    unsigned int done : 1;
};

union Pair {
    long long whole;
    int word[2];
};

extern "C" {
void func_02020c98(unsigned char *buf);
void func_02020ca8(unsigned char *buf);
unsigned char *func_02020d78(unsigned char *buf, unsigned int id, int key);
unsigned int func_02020e54(unsigned char *p);
int func_0202037c(unsigned char *p);
}

extern "C" void func_0202152c(int param_1, int param_2, Source *param_3)
{
    unsigned char *found;
    unsigned int flag;
    unsigned int i;
    int entry;
    int base;
    unsigned char buf[8];

    func_02020c98(buf);
    param_3->slot3(buf);
    base = param_2 + 8;
    flag = 0x7fffffffu + 1;
    for (i = 0; i < ((Header *)param_2)->count; i++) {
        entry = base + i * 0x28;
        found = func_02020d78(buf, *(unsigned int *)(entry + 4) & 0xff, *(int *)(base + i * 0x28));
        if (found != 0) {
            *(unsigned int *)(entry + 4) = 0x7fffffff & (0xffff00ff & *(unsigned int *)(entry + 4) | (func_02020e54(found) & 0xff) << 8);
            if (func_0202037c(found) != 0) {
                *(Pair *)((char *)entry + 0x18) = *(Pair *)(found + 0x1c);
                *(Pair *)((char *)entry + 0x20) = *(Pair *)(found + 0x24);
                *(Pair *)((char *)entry + 8) = *(Pair *)(found + 0xc);
                *(Pair *)((char *)entry + 0x10) = *(Pair *)(found + 0x14);
            }
            else {
                *(int *)(entry + 8) = *(int *)(found + 0xc) - (int)param_3;
                *(int *)(entry + 0xc) = *(int *)(found + 0x14);
            }
        }
        else {
            *(unsigned int *)(entry + 4) = flag | (*(unsigned int *)(entry + 4) & 0x7fffffff);
        }
    }
    ((Header *)param_2)->done = 1;
    func_02020ca8(buf);
}

struct Panel {
    unsigned char pad000[0x30];
    long long w30;
    unsigned char pad038[0xdc - 0x38];
    unsigned int wdc;
    unsigned int we0;
    unsigned char pad0e4[0xf8 - 0xe4];
    int wf8;
    unsigned char pad0fc[0x9cc - 0xfc];
    int w9cc;
    int w9d0;
    int w9d4;
    unsigned long long now;
    unsigned long long prev;
    unsigned long long ring[16];
    int frames;
    unsigned int wa6c;
    int count;
    int limit;
    unsigned char pada78[0xaa8 - 0xa78];
    unsigned char aa8_low:7;
    unsigned char aa8_b7:1;
    unsigned char aa9;
    unsigned char mode:3;
};

extern volatile unsigned long long data_020bbb58;
extern unsigned int data_020bbb54;
extern unsigned int func_ov004_020d3ce4(int a);
extern int func_ov004_020d3e58(int a);
extern void func_ov004_020d5c10(struct Panel *p);
extern unsigned long long func_020753bc(void);

void func_ov004_020d6704(struct Panel *param_1)
{
    int diff;

    if ((param_1->mode & 4) != 0) {
        if (param_1->we0 >= param_1->wdc || func_ov004_020d3ce4(param_1->w9d4) == 0) {
            diff = param_1->w30 - data_020bbb58;
            if (param_1->aa8_b7 || (diff <= param_1->wf8 * 3 && param_1->count < (param_1->limit < 1 ? 1 : param_1->limit) && func_ov004_020d3ce4(param_1->w9d4) != 0)) {
                if ((param_1->mode & 1) != 0) {
                    func_ov004_020d3e58(param_1->w9cc);
                }
                if ((param_1->mode & 2) != 0) {
                    func_ov004_020d3e58(param_1->w9d0);
                }
                param_1->count++;
            } else {
                param_1->count = 0;
                func_ov004_020d5c10(param_1);
            }
        }
    } else if (data_020bbb54 > param_1->wa6c && param_1->count < param_1->limit) {
        if ((param_1->mode & 1) != 0) {
            func_ov004_020d3e58(param_1->w9cc);
        }
        if ((param_1->mode & 2) != 0) {
            func_ov004_020d3e58(param_1->w9d0);
        }
        param_1->count++;
    } else {
        param_1->count = 0;
        func_ov004_020d5c10(param_1);
    }
    param_1->prev = param_1->now;
    param_1->now = func_020753bc();
    if (param_1->frames > 0) {
        param_1->ring[(param_1->frames - 1) % 16] = param_1->now - param_1->prev;
    }
    param_1->frames++;
}

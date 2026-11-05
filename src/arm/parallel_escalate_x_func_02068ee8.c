typedef struct {
    unsigned int size;
    unsigned int sizeA;
    unsigned int sizeB;
} Bank;

typedef struct {
    Bank bank[4];
} BankTable;

typedef struct {
    unsigned int head;
    unsigned int f4;
    unsigned int total;
    unsigned int addr;
    unsigned int len;
} Heap;

typedef struct {
    unsigned int list0;
    unsigned int list1;
    Heap heap;
} Manager;

extern const BankTable data_0209e2dc;
extern Manager data_020cff8c;
extern unsigned int data_020cff90[];

extern void func_02068a8c(unsigned int *list);
extern unsigned int func_02068a98(unsigned int addr, unsigned int len);
extern void func_02068adc(unsigned int *list, Heap *heap, unsigned int offset, unsigned int size);
extern void func_02068c84(unsigned int *list, Heap *heap);

void func_02068ee8(void)
{
    BankTable t = data_0209e2dc;
    Heap *heap = &data_020cff8c.heap;
    unsigned int other;
    unsigned int rest;
    unsigned int half;
    unsigned int i;
    unsigned int n;
    unsigned int s0;
    unsigned int s2;

    rest = data_020cff8c.heap.total;
    other = data_020cff8c.heap.f4 - (rest + (rest >> 1));
    half = rest >> 1;
    for (i = 0; i < 4; i++) {
        if (i == 0 || i == 2) {
            n = t.bank[i].size;
            if (n != 0 && rest != 0) {
                if (n > rest) {
                    n = rest;
                }
                t.bank[i].sizeB += n;
                rest -= n;
                t.bank[i].size -= n;
            }
        }
    }
    t.bank[1].size -= half;
    i = 0;
    if (half != 0) {
        i = 0;
    }
    do {
        n = t.bank[i].size;
        if (n != 0 && other != 0) {
            if (n > other) {
                n = other;
            }
            t.bank[i].sizeA += n;
            other -= n;
            t.bank[i].size -= n;
        }
        i++;
    } while (i < 4);

    func_02068a8c(&data_020cff90[-1]);
    func_02068a8c(&data_020cff90[0]);
    data_020cff8c.heap.head = func_02068a98(data_020cff8c.heap.addr, data_020cff8c.heap.len >> 4);

    s0 = t.bank[0].sizeB;
    if (s0 != 0) {
        func_02068adc(&data_020cff90[0], &data_020cff8c.heap, 0, s0);
    }
    if (t.bank[0].sizeA != 0) {
        func_02068adc(&data_020cff90[-1], &data_020cff8c.heap, s0, t.bank[0].sizeA);
    }
    s2 = t.bank[2].sizeB;
    if (s2 != 0) {
        func_02068adc(&data_020cff90[0], &data_020cff8c.heap, 0x40000, s2);
    }
    if (t.bank[2].sizeA != 0) {
        func_02068adc(&data_020cff90[-1], &data_020cff8c.heap, s2 + 0x40000, t.bank[2].sizeA);
    }
    if (t.bank[3].sizeA != 0) {
        func_02068adc(&data_020cff90[-1], &data_020cff8c.heap, 0x60000, t.bank[3].sizeA);
    }
    if (t.bank[1].sizeA != 0) {
        func_02068adc(&data_020cff90[-1], &data_020cff8c.heap, half + 0x20000, t.bank[1].sizeA);
    }
    func_02068c84(&data_020cff90[-1], heap);
    func_02068c84(&data_020cff90[0], heap);
}

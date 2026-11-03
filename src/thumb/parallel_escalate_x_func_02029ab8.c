typedef struct Step29ab8 {
    unsigned short a;
    unsigned short b;
    unsigned short c;
} Step29ab8;

typedef struct Obj29ab8 {
    int f0;
    int f4;
    unsigned short *table;
    char pad[0x402];
    unsigned short group;
} Obj29ab8;

extern void func_02029c18(Obj29ab8 *object, Step29ab8 *step);

void func_02029ab8(Obj29ab8 *object)
{
    Step29ab8 step;
    unsigned short *table = object->table;
    unsigned int offset = table[object->group];
    unsigned short *entry = &table[offset];
    int count = *entry++;
    while (count != 0) {
        step.a = entry[0];
        step.b = entry[1];
        step.c = entry[2];
        func_02029c18(object, &step);
        entry += 3;
        count--;
    }
}

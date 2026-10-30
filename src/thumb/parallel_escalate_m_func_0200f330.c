typedef struct Flags {
    char pad[8];
    unsigned int a : 1;
    unsigned int b : 1;
    unsigned int c : 1;
    unsigned int d : 1;
    unsigned int rest : 28;
    int pad2;
    unsigned int lo : 18;
    unsigned int e : 1;
    unsigned int hi : 13;
} Flags;

int func_0200f330(Flags *f)
{
    if (f->a || f->b || f->d || f->e) {
        return 1;
    }
    return 0;
}

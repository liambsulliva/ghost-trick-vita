typedef struct Rec2ec94 {
    int a;
    int b;
    int c;
    unsigned char d;
} Rec2ec94;

void func_0202ec94(Rec2ec94 *dest, const Rec2ec94 *src)
{
    int a = src->a;
    dest->b = src->b;
    dest->a = a;
    dest->c = src->c;
    dest->d = src->d;
}

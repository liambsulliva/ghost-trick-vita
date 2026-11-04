typedef struct Rec2ee7c {
    int a;
    int b;
    int c;
    unsigned char d;
} Rec2ee7c;

void func_0202ee7c(Rec2ee7c *dest, const Rec2ee7c *src)
{
    int a = src->a;
    dest->b = src->b;
    dest->a = a;
    dest->c = src->c;
    dest->d = src->d;
}

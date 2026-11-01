typedef struct Rec47b3c {
    unsigned char a;
    unsigned char b;
    unsigned char c;
    unsigned char d;
    int value;
} Rec47b3c;

void func_02047b3c(Rec47b3c *dest, const Rec47b3c *src)
{
    dest->a = src->a;
    dest->b = src->b;
    dest->c = src->c;
    dest->d = src->d;
    dest->value = src->value;
}

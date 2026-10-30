typedef struct {
    unsigned char pad[31];
    unsigned char low : 4;
} Bits02053044;

unsigned int func_02053044(char *part)
{
    return ((Bits02053044 *)part)->low;
}

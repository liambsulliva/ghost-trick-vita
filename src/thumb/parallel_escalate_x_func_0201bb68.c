typedef struct Obj1bb68 {
    char pad[0x2a];
    unsigned short color;
    char pad2[0x3c];
    unsigned char *pixels;
    char pad3[8];
    unsigned short width;
    char pad4[2];
    unsigned int size;
} Obj1bb68;

void func_0201bb68(Obj1bb68 *object, unsigned int left, unsigned int right, int row)
{
    unsigned int x;
    for (x = left >> 1; x <= right >> 1; x++) {
        unsigned int offset = row * object->width * 4;
        if (x + offset < object->size) {
            object->pixels[offset + x] = object->color | (object->color << 4);
        }
    }
}

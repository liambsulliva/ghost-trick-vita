typedef struct Entry304f8 {
    unsigned int src;
    unsigned int size;
    unsigned int palette;
} Entry304f8;

typedef struct Dest304f8 {
    unsigned short width;
    unsigned short height;
    unsigned int dest;
    unsigned int palette;
} Dest304f8;

extern Entry304f8 data_02086618[];
extern Dest304f8 data_020cc1a8;
extern void func_02068da0(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, int param_5);
extern void func_02069108(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);

void func_020304f8(int index)
{
    Entry304f8 *entry = &data_02086618[index];
    func_02068da0(entry->src, entry->size, data_020cc1a8.dest, data_020cc1a8.width, 1);
    func_02069108(entry->palette, data_020cc1a8.palette, data_020cc1a8.height, 1);
}

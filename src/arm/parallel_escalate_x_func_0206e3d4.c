typedef struct {
    char pad0[0x14];
    unsigned int flag;
} Archive;

typedef struct {
    char pad0[0x40];
    unsigned int fnt_offset;
    unsigned int fnt_size;
    unsigned int fat_offset;
    unsigned int fat_size;
} RomHeader;

extern unsigned int data_020d0ddc[2];
extern Archive data_020d0de4;
extern char data_020b1738[];
extern char data_020b173c[];

extern void func_02069a68(void);
extern unsigned int func_02073a04(void);
extern void func_0206bfbc(Archive *arc);
extern int func_0206bfe0(Archive *arc, const char *name, unsigned int length);
extern int func_02075d50(void);
extern RomHeader *func_02069bb4(void);
extern void func_0206e0b4(Archive *arc, void *proc, unsigned int flags);
extern int func_0206dfc8(Archive *arc, unsigned int base, unsigned int fat, unsigned int fat_size, unsigned int fnt, unsigned int fnt_size, void *read_func, void *write_func);
extern void func_0206e3cc(Archive *arc);
extern int func_0206bba0(const char *path);
extern int func_0206e338();
extern int func_0206e2f0();
extern int func_0206e3a8();
extern int func_0206e3bc();
extern int func_0206e3c4();

static inline int func_0206e3d4_is_loaded(volatile const Archive *arc)
{
    return (arc->flag & 2) ? 1 : 0;
}

void func_0206e3d4(unsigned int dma_no)
{
    func_02069a68();
    data_020d0ddc[0] = dma_no;
    data_020d0ddc[1] = func_02073a04();
    func_0206bfbc(&data_020d0de4);
    func_0206bfe0(&data_020d0de4, data_020b1738, 3);
    if (func_02075d50() == 1) {
        RomHeader *header = func_02069bb4();
        RomHeader *header2 = func_02069bb4();
        func_0206e0b4(&data_020d0de4, func_0206e338, 0x682);
        if (header->fnt_offset != 0xffffffff && header->fnt_offset != 0 && header2->fat_offset != 0xffffffff && header2->fat_offset != 0) {
            func_0206dfc8(&data_020d0de4, 0, header2->fat_offset, header2->fat_size, header->fnt_offset, header->fnt_size, func_0206e2f0, 0);
        }
    } else {
        func_0206e3cc(&data_020d0de4);
    }
    if (!func_0206e3d4_is_loaded(&data_020d0de4)) {
        func_0206e0b4(&data_020d0de4, func_0206e3a8, 0xffffffff);
        func_0206dfc8(&data_020d0de4, 0, 0, 0, 0, 0, func_0206e3bc, func_0206e3c4);
    }
    func_0206bba0(data_020b173c);
}

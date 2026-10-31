typedef struct {
    unsigned int offset;
    unsigned int length;
} RomRegion;

typedef struct {
    char pad[0x50];
    RomRegion main_ovt;
    RomRegion sub_ovt;
} RomHeader;

typedef struct {
    void *arc;
    unsigned int file_id;
} FileID;

typedef struct {
    unsigned int id;
    void *ram_address;
    unsigned int ram_size;
    unsigned int bss_size;
    void *sinit_init;
    void *sinit_init_end;
    unsigned int file_id;
    unsigned int compressed;
    int target;
    unsigned int file_pos;
    unsigned int file_size;
} OverlayInfo;

typedef struct {
    int body[18];
} File;

extern RomRegion data_020d0dc4;
extern RomRegion data_020d0dcc;

extern void func_0206c440(File *file);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern FileID func_0206c8c0(const OverlayInfo *p_ovi);
extern int func_0206c538(File *file, FileID id);
extern RomHeader *func_02069bb4(void);
extern int func_0206e548(File *file, unsigned int top, unsigned int bottom);
extern int func_0206c704(File *file, void *dst, int len);
extern int func_0206c614(File *file);
extern unsigned int func_0206e2bc(File *file);
extern unsigned int func_0206c628(File *file);

int func_0206c8ec(OverlayInfo *p_ovi, int target, unsigned int id)
{
    int retval = 0;
    const unsigned int pos = id * 32;
    RomRegion *pr = (target == 0) ? &data_020d0dc4 : &data_020d0dcc;

    if (pr->offset) {
        if (pos < pr->length) {
            File file[1];
            func_0206c440(file);
            func_0207303c((const void *)(pr->offset + pos), p_ovi, 32);
            p_ovi->target = target;
            if (func_0206c538(file, func_0206c8c0(p_ovi))) {
                p_ovi->file_pos = func_0206e2bc(file);
                p_ovi->file_size = func_0206c628(file);
                func_0206c614(file);
                retval = 1;
            }
        }
    } else {
        if (target == 0) {
            pr = &func_02069bb4()->main_ovt;
        } else {
            pr = &func_02069bb4()->sub_ovt;
        }
        if (pos < pr->length) {
            File file[1];
            func_0206c440(file);
            if (func_0206e548(file, pr->offset + pos, pr->offset + pr->length)) {
                if (func_0206c704(file, p_ovi, 32) != 32) {
                    func_0206c614(file);
                } else {
                    func_0206c614(file);
                    p_ovi->target = target;
                    if (func_0206c538(file, func_0206c8c0(p_ovi))) {
                        p_ovi->file_pos = func_0206e2bc(file);
                        p_ovi->file_size = func_0206c628(file);
                        func_0206c614(file);
                        retval = 1;
                    }
                }
            }
        }
    }
    return retval;
}

typedef struct {
    unsigned int total_size;
    unsigned int sect_size;
    unsigned int subsect_size;
    unsigned int page_size;
    unsigned int addr_width;
    unsigned int program_page;
    unsigned int write_page;
    unsigned int write_page_total;
    unsigned int erase_chip;
    unsigned int erase_chip_total;
    unsigned int erase_sector;
    unsigned int erase_sector_total;
    unsigned int erase_subsector;
    unsigned int erase_subsector_total;
    unsigned int erase_page;
    unsigned char initial_status;
    unsigned char pad[3];
    unsigned int caps;
    unsigned int reserved;
} BackupSpec;

typedef struct {
    int result;
    int type;
    unsigned int pad[4];
    BackupSpec spec;
} CommandArg;

typedef struct {
    CommandArg *cmd;
} CardCommon;

extern CardCommon data_020d02a0;

extern void func_02072fa8(void *dst, unsigned char val, unsigned int size);

void func_0206a408(int type)
{
    CommandArg *const p = data_020d02a0.cmd;
    func_02072fa8(&p->spec, 0, sizeof(p->spec));
    p->type = type;
    p->spec.caps = 0x203f;
    if (type != 0) {
        const unsigned int size = (unsigned int)(1 << ((type >> 8) & 0xff));
        const int device = type & 0xff;
        const int vender = (type >> 16) & 0xff;
        p->spec.total_size = size;
        p->spec.initial_status = 0xff;
        if (device == 1) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x000200:
                p->spec.page_size = 0x10;
                p->spec.addr_width = 1;
                p->spec.program_page = 5;
                p->spec.initial_status = 0xf0;
                break;
            case 0x002000:
                p->spec.page_size = 0x20;
                p->spec.addr_width = 2;
                p->spec.program_page = 5;
                p->spec.initial_status = 0x00;
                break;
            case 0x010000:
                p->spec.page_size = 0x80;
                p->spec.addr_width = 2;
                p->spec.program_page = 10;
                p->spec.initial_status = 0x00;
                break;
            case 0x020000:
                p->spec.page_size = 0x100;
                p->spec.addr_width = 3;
                p->spec.program_page = 5;
                p->spec.initial_status = 0x00;
                break;
            }
            p->spec.sect_size = p->spec.page_size;
            p->spec.caps |= 0x0040;
            p->spec.caps |= 0x0100;
            p->spec.caps |= 0x0200;
            p->spec.caps |= 0x4000;
        } else if (device == 2) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x040000:
            case 0x080000:
            case 0x100000:
                p->spec.write_page = 25;
                p->spec.write_page_total = 300;
                p->spec.erase_page = 300;
                p->spec.erase_sector = 5000;
                p->spec.caps |= 0x0080;
                p->spec.caps |= 0x0400;
                break;
            case 0x200000:
                p->spec.write_page = 23;
                p->spec.write_page_total = 300;
                p->spec.erase_sector = 500;
                p->spec.erase_sector_total = 5000;
                p->spec.erase_chip = 10000;
                p->spec.erase_chip_total = 60000;
                p->spec.initial_status = 0x00;
                p->spec.caps |= 0x0080;
                p->spec.caps |= 0x0400;
                p->spec.caps |= 0x1000;
                p->spec.caps |= 0x4000;
                break;
            case 0x400000:
                p->spec.erase_sector = 600;
                p->spec.erase_sector_total = 3000;
                p->spec.erase_subsector = 70;
                p->spec.erase_subsector_total = 150;
                p->spec.erase_chip = 23000;
                p->spec.erase_chip_total = 800000;
                p->spec.initial_status = 0x00;
                p->spec.subsect_size = 0x1000;
                p->spec.caps |= 0x1000;
                p->spec.caps |= 0x4000;
                p->spec.caps |= 0x8000;
                break;
            case 0x800000:
                if (vender == 0) {
                    p->spec.erase_sector = 1000;
                    p->spec.erase_sector_total = 3000;
                    p->spec.erase_chip = 68000;
                    p->spec.erase_chip_total = 160000;
                    p->spec.initial_status = 0x00;
                    p->spec.caps |= 0x1000;
                    p->spec.caps |= 0x4000;
                } else if (vender == 1) {
                    p->spec.erase_sector = 1000;
                    p->spec.erase_sector_total = 3000;
                    p->spec.erase_chip = 68000;
                    p->spec.erase_chip_total = 160000;
                    p->spec.initial_status = 0x84;
                    p->spec.caps |= 0x1000;
                    p->spec.caps |= 0x4000;
                }
                break;
            }
            p->spec.sect_size = 0x10000;
            p->spec.page_size = 0x100;
            p->spec.addr_width = 3;
            p->spec.program_page = 5;
            p->spec.caps |= 0x0040;
            p->spec.caps |= 0x0100;
            p->spec.caps |= 0x0200;
            p->spec.caps |= 0x0800;
        } else if (device == 3) {
            switch (size) {
            default:
                goto invalid_type;
            case 0x002000:
            case 0x008000:
                break;
            }
            p->spec.page_size = size;
            p->spec.sect_size = size;
            p->spec.addr_width = 2;
            p->spec.initial_status = 0x00;
            p->spec.caps |= 0x0040;
            p->spec.caps |= 0x0100;
            p->spec.caps |= 0x0200;
            p->spec.caps |= 0x4000;
        } else {
        invalid_type:
            p->type = 0;
            p->spec.total_size = 0;
            data_020d02a0.cmd->result = 3;
            return;
        }
    }
}

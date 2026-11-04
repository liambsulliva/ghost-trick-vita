extern "C" {
extern unsigned char data_02084d1c[];
extern unsigned char data_02084d28[];
extern unsigned char data_02084d38[];

void func_0201ef58(void *archive);
void func_0201ef74(void *archive);
int func_0201efb4(void *archive, void *name, int flag);
void func_0201efd8(void *archive);
void func_0201f014(void *archive, unsigned char *dst, unsigned int size);
unsigned int func_0201f02c(void *archive);
}

struct Archive {
    unsigned char data[80];
    Archive() { func_0201ef58(this); }
    ~Archive() { func_0201ef74(this); }
};

extern "C" void func_0201d3a4(unsigned char *board, int index)
{
    Archive arc;
    unsigned char *name;
    unsigned int size;

    switch (index) {
    case 0:
        name = data_02084d1c;
        break;
    case 1:
        name = data_02084d28;
        break;
    case 2:
        name = data_02084d38;
        break;
    default:
        return;
    }
    if (func_0201efb4(&arc, name, 1) == 0) {
        return;
    }
    size = func_0201f02c(&arc);
    if (index == 2) {
        if (size > 0xe40) {
            size = 0xe40;
        }
        func_0201f014(&arc, board + 0xec4, size);
    } else {
        if (size > 0x720) {
            size = 0x720;
        }
        if (index == 0) {
            func_0201f014(&arc, board + 0x84, size);
        } else if (index == 1) {
            func_0201f014(&arc, board + 0x7a4, size);
        }
    }
    func_0201efd8(&arc);
}

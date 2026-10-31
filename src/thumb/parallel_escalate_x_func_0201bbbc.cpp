class Source {
public:
    virtual void s0();
    virtual void s1();
    virtual unsigned short (*load(int id, void *data))[32];
};
struct Grid {
    unsigned char pad[0x70];
    unsigned short (*map)[32];
    unsigned short width;
    unsigned char height;
    unsigned char pad77[0x7c - 0x77];
    int id;
};
extern "C" {
extern char data_02084cd8[];
void *func_02007bd8(void);
Source *func_02007b70(void *owner);
void func_0201bc84(Grid *grid);

void func_0201bbbc(Grid *grid)
{
    unsigned int i;
    unsigned int j;
    unsigned int y;
    unsigned int x;
    unsigned int row;
    unsigned int col;
    if (grid->map == 0) {
        grid->map = func_02007b70(func_02007bd8())->load(grid->id, data_02084cd8);
    }
    func_0201bc84(grid);
    for (i = 0; i < grid->height; i++) {
        for (j = 0; j < grid->width; j++) {
            grid->map[i][j] = j + i * grid->width + 0x100;
        }
    }
    for (y = 0; y < grid->height; y++) {
        for (x = grid->width; x < 0x20; x++) {
            grid->map[y][x] = grid->width * grid->height + 0x100;
        }
    }
    for (row = grid->height; row < 0x20; row++) {
        for (col = 0; col < 0x20; col++) {
            grid->map[row][col] = grid->width * grid->height + 0x100;
        }
    }
}
}

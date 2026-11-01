typedef struct Names0bb1c {
    unsigned short ids[5];
} Names0bb1c;

typedef struct Glyph0bb1c {
    unsigned short flags : 2;
    unsigned short index : 14;
} Glyph0bb1c;

typedef struct Cell0bb1c {
    char data[8];
} Cell0bb1c;

typedef struct Lang0bb1c {
    unsigned char lang;
} Lang0bb1c;

extern Names0bb1c data_02086f84[];
extern Lang0bb1c data_020cff60;
extern Glyph0bb1c data_02088910[];
extern Cell0bb1c data_0208c064[];

Cell0bb1c *func_0200bb1c(void *self, unsigned int code)
{
    if (code & 0x8000) {
        code &= ~0x8000;
        code = data_02086f84[code].ids[data_020cff60.lang - 1];
    }
    return &data_0208c064[data_02088910[code].index];
}

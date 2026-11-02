typedef struct Calibrate77cc4 {
    short x0;
    short y0;
    short x_dot;
    short y_dot;
} Calibrate77cc4;

extern int func_02075c98(void);
extern void func_02075cac(int enabled);

static inline void func_02077cc4_set_div(unsigned int numer, unsigned int denom)
{
    *(volatile unsigned short *)0x04000280 = 0;
    *(unsigned int *)0x04000290 = numer;
    *(unsigned long long *)0x04000298 = denom;
}

static inline int func_02077cc4_get_div(void)
{
    while (*(volatile unsigned short *)0x04000280 & 0x8000) {
    }
    return *(volatile int *)0x040002a0;
}

unsigned int func_02077cc4(Calibrate77cc4 *calibrate, unsigned short raw_x1, unsigned short raw_y1, unsigned short dx1, unsigned short dy1,
                           unsigned short raw_x2, unsigned short raw_y2, unsigned short dx2, unsigned short dy2)
{
    int rx_width;
    int ry_width;
    int dx_width;
    int dy_width;
    int tmp;
    int enabled;

    if (raw_x1 >= 0x1000 || raw_y1 >= 0x1000 || raw_x2 >= 0x1000 || raw_y2 >= 0x1000) {
        return 1;
    }
    if (dx1 >= 256 || dx2 >= 256 || dy1 >= 192 || dy2 >= 192) {
        return 1;
    }
    if (dx1 == dx2 || dy1 == dy2 || raw_x1 == raw_x2 || raw_y1 == raw_y2) {
        return 1;
    }
    enabled = func_02075c98();
    rx_width = raw_x1 - raw_x2;
    ry_width = raw_y1 - raw_y2;
    dx_width = dx1 - dx2;
    dy_width = dy1 - dy2;
    func_02077cc4_set_div(rx_width << 8, dx_width);
    tmp = func_02077cc4_get_div();
    func_02077cc4_set_div(ry_width << 8, dy_width);
    if (tmp >= 0x8000 || tmp < -0x8000) {
        func_02075cac(enabled);
        return 1;
    }
    calibrate->x_dot = tmp;
    tmp = (short)((((raw_x1 + raw_x2) << 8) - calibrate->x_dot * (dx1 + dx2)) >> 7);
    if (tmp >= 0x8000 || tmp < -0x8000) {
        func_02075cac(enabled);
        return 1;
    }
    calibrate->x0 = tmp;
    tmp = func_02077cc4_get_div();
    func_02075cac(enabled);
    if (tmp >= 0x8000 || tmp < -0x8000) {
        return 1;
    }
    calibrate->y_dot = tmp;
    tmp = (short)((((raw_y1 + raw_y2) << 8) - calibrate->y_dot * (dy1 + dy2)) >> 7);
    if (tmp >= 0x8000 || tmp < -0x8000) {
        return 1;
    }
    calibrate->y0 = tmp;
    return 0;
}

struct Pt {
    short x;
    short y;
    short z;
    short w;
};

struct Quad {
    char pad[0x7c];
    struct Pt pts[4];
    char pad2[0xc4 - 0x9c];
    unsigned char mode : 4;
    unsigned char rest : 4;
};

void func_02057308(struct Quad *q, unsigned char mode)
{
    int i;
    int m;

    if (q->mode != mode) {
        q->mode = mode;
        m = q->mode;
        if (m == 0) {
            short mx = (short)((q->pts[0].x + q->pts[2].x) >> 1);
            short my = (short)((q->pts[0].y + q->pts[2].y) >> 1);
            int j;
            for (j = 0; j < 4; j++) {
                q->pts[j].x -= mx;
                q->pts[j].y -= my;
            }
            return;
        }
        if (m == 1) {
            for (i = 1; i < 4; i++) {
                q->pts[i].x -= q->pts[0].x;
                q->pts[i].y -= q->pts[0].y;
            }
            q->pts[0].x = q->pts[0].y = 0;
            return;
        }
        if (m == 2) {
            for (i = 0; i < 4; i++) {
                if (i != 1) {
                    q->pts[i].x -= q->pts[1].x;
                    q->pts[i].y -= q->pts[1].y;
                }
            }
            q->pts[1].x = q->pts[1].y = 0;
            return;
        }
        if (m == 3) {
            for (i = 0; i < 3; i++) {
                q->pts[i].x -= q->pts[3].x;
                q->pts[i].y -= q->pts[3].y;
            }
            q->pts[3].x = q->pts[3].y = 0;
            return;
        }
        if (m == 4) {
            for (i = 0; i < 4; i++) {
                if (i != 2) {
                    q->pts[i].x -= q->pts[2].x;
                    q->pts[i].y -= q->pts[2].y;
                }
            }
            q->pts[2].x = q->pts[2].y = 0;
        }
    }
}

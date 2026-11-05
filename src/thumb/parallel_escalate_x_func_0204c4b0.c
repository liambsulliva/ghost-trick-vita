typedef struct Mover0204c4b0 {
    char pad[0x34];
    int speed;
    char pad38[0x84];
    int pos;
    int vel;
} Mover0204c4b0;

void func_0204c4b0(Mover0204c4b0 *self)
{
    if (self->speed != 0) {
        self->pos += self->vel;
        if (self->pos > 0x3d000 / self->speed + 0xa29) {
            self->vel = -0x3d000 / self->speed;
            self->pos = 0x3d000 / self->speed + 0xa29;
        } else if (self->pos < -0x3d000 / self->speed + 0xa29) {
            self->vel = 0x3d000 / self->speed;
            self->pos = -0x3d000 / self->speed + 0xa29;
        }
    }
}

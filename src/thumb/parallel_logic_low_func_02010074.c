struct Game {
    char pad0[0xc];
    unsigned int flag : 1;
};

void func_02010074(struct Game *game, int value)
{
    game->flag = value != 0;
}

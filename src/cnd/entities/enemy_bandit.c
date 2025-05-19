#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/mesh.h>
#include <cnd/xutils.h>

void routine_bandit_stolen(entity e) 
{
    const i16 dy = CAMERA.position.y - e->position.y;
    const i16 dx = e->position.x - CAMERA.position.x;
    e->velocity.x = ((dx < 0) - (dx > 0)) << 1;
    e->velocity.y = ((dy < 0) - (dy > 0)) << 1;

    if (e->isLocal)
    {
        const i8 dx8 = I8(dx);
        const i8 dy8 = I8(dy);
        if (dy >= 0)
            routine_enemy_collision(e, dx8, dy8, spider_right_up, spider_left_up);
        else
            routine_enemy_collision(e, dx8, dy8, spider_right_down, spider_left_down);
    }
}
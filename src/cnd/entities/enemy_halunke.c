#include <cnd/world.h>
#include <cnd/entities.h>
#include <cnd/globals.h>
#include <cnd/xutils.h>
#include <cnd/mesh.h>

void routine_halunke_follow(entity e)
{
    if (e->isLocal)
    {
        const i16 dx = e->position.x - CAMERA.position.x;
        const i16 dy = CAMERA.position.y - e->position.y;
        if (dx < -2 || dx > 2)
            e->velocity.x = ((dx < 0) - (dx > 0)) << 1;
        routine_enemy_collision(e, I8(dx), I8(dy), halunke_left, halunke_right);
    }
}